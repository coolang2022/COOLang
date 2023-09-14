from extractTrainingDataFromJsonFile import *
from embeddingTrainingData import *
import numpy as np

# for training
import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader, SequentialSampler
import torch.optim as optim
import torch.nn.functional as F
import matplotlib.pyplot as plt
from torch.autograd import Function


# for testing
from torch.utils.data.dataset import random_split
from torch.utils.data import DataLoader


# Hyperparameters
CODE_EMBEDDING_SIZE = 55 - 3 * 8
# CODE_EMBEDDING_SIZE = int(
#     extended_code_embedding(
#         extended_grounding_state_list[0].extendedCodeTable.extendedCodeList[0]
#     ).__len__()
# )
code_segment_length = 10
input_size = code_segment_length * (
    CODE_EMBEDDING_SIZE
)  # Assuming each code embedding has a size of YOUR_EMBEDDING_SIZE and codeSegmentLength=5
hidden_size = int(31 * 5)
output_size = int(20)
num_layers = 2
num_epochs = 15
batch_size = 1
learning_rate = 0.0001

pad_token = -11
heads = 5

code_segment_embedding_data = []  # store the codeseg and corresponding embedding

# Define the device here for global accessibility
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")


class MultiHeadAttention(nn.Module):
    def __init__(self, embed_size, heads):
        super(MultiHeadAttention, self).__init__()
        self.embed_size = embed_size
        self.heads = heads
        self.head_dim = embed_size // heads

        assert (
            self.head_dim * heads == embed_size
        ), "Embedding size needs to be divisible by heads"

        self.values = nn.Linear(self.head_dim, self.head_dim, bias=False)
        self.keys = nn.Linear(self.head_dim, self.head_dim, bias=False)
        self.queries = nn.Linear(self.head_dim, self.head_dim, bias=False)
        self.fc_out = nn.Linear(heads * self.head_dim, embed_size)

    def forward(self, values, keys, query, mask):
        N = query.shape[0]
        value_len, key_len, query_len = values.shape[1], keys.shape[1], query.shape[1]

        # Split the embedding into self.heads different pieces
        values = values.reshape(N, value_len, self.heads, self.head_dim)
        keys = keys.reshape(N, key_len, self.heads, self.head_dim)
        queries = query.reshape(N, query_len, self.heads, self.head_dim)

        values = self.values(values)
        keys = self.keys(keys)
        queries = self.queries(queries)

        # Scaled dot-product attention
        energy = torch.einsum("nqhd,nkhd->nhqk", [queries, keys]) / (
            self.embed_size ** (1 / 2)
        )
        if mask is not None:
            energy = energy.masked_fill(mask == 0, float("-1e20"))

        attention = torch.nn.functional.softmax(energy, dim=3)

        out = torch.einsum("nhql,nlhd->nqhd", [attention, values]).reshape(
            N, query_len, self.heads * self.head_dim
        )

        out = self.fc_out(out)
        return out


def object_to_dict(obj):
    # If obj is an object, convert it to a dictionary
    if hasattr(obj, "__dict__"):
        obj_dict = {}
        # For each attribute, recursively convert it to a dictionary if it's an object
        for key, value in obj.__dict__.items():
            obj_dict[object_to_dict(key)] = object_to_dict(value)
        return obj_dict
    # If obj is a list, recursively convert its items
    elif isinstance(obj, list):
        return [object_to_dict(item) for item in obj]
    # If obj is a set, convert it to a list
    elif isinstance(obj, set):
        return object_to_dict(list(obj))
    # If obj is a PyTorch Tensor, convert it to a list
    elif isinstance(obj, torch.Tensor):
        return object_to_dict(obj.tolist())
    # Otherwise, return the object as it is
    else:
        return obj


class CodeSegmentDataset(Dataset):
    def __init__(
        self,
        extended_grounding_state_list: list[ExtendedGroundingState],
        code_segment_length=5,
        label_length=output_size,
    ):
        self.data: list[ExtendedGroundingState] = extended_grounding_state_list
        self.code_segment_length = code_segment_length
        self.label_length = label_length

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        extended_grounding_state = self.data[idx]
        extended_code_table = extended_grounding_state.extendedCodeTable
        extended_code_embeddings = [
            torch.tensor(extended_code_embedding(extended_code) + [-12])
            for extended_code in extended_code_table.extendedCodeList[::]
        ]
        extended_code_embeddings[-1][-1] = -13

        # Make sure every input is of fixed length by padding or truncating
        extended_code_segments = []
        if len(extended_code_embeddings) <= self.code_segment_length:
            segment_tensor = extended_code_embeddings
            while (
                len(segment_tensor) < self.code_segment_length
            ):  # Just to ensure that the last segment has the required length, though it might not be necessary
                segment_tensor.append([pad_token] * segment_tensor[0].__len__())

            extended_code_segments = torch.cat(
                [torch.tensor(sublist) for sublist in segment_tensor], dim=0
            )
            extended_code_segments = torch.unsqueeze(extended_code_segments, dim=0)
        else:
            for i in range(
                0,
                len(extended_code_embeddings) - self.code_segment_length + 1,
                self.code_segment_length,
            ):
                segment_tensor = extended_code_embeddings[
                    i : min(i + self.code_segment_length, len(extended_code_embeddings))
                ]
                while (
                    len(segment_tensor[-1]) < self.code_segment_length
                ):  # Just to ensure that the last segment has the required length, though it might not be necessary
                    segment_tensor[-1].extend(
                        [pad_token] * (self.code_segment_length - len(segment_tensor))
                    )
                    # segment_tensor.append(torch.zeros_like(segment_tensor[0]))
                segment_tensor = torch.cat(
                    [sublist.clone().detach() for sublist in segment_tensor], dim=0
                )
                extended_code_segments.append(segment_tensor)
            extended_code_segments = torch.stack(extended_code_segments, dim=0)

        # Convert list of tensors to a single tensor with an extra sequence length dimension
        # extended_code_embeddings_tensor = torch.stack(extended_code_embeddings)

        
        label = output_embedding(extended_grounding_state)
        if label.__len__() < self.label_length:
            label.extend([0] * (self.label_length - label.__len__()))
        label = torch.tensor(label[::], dtype=torch.float32)

        # data_dict = {
        #     "extended_grounding_state": object_to_dict(extended_grounding_state),
        #     "extended_code_embeddings": object_to_dict(extended_code_embeddings),
        #     "extended_code_segments": extended_code_segments.tolist().__str__(),
        #     "labels": label.__str__(),
        # }
        # code_segment_embedding_data.append(data_dict)

        return extended_code_segments, label

    def _one_hot_encode(self, position, size):
        # Helper function to convert position into one-hot vector
        one_hot = torch.zeros(size)
        one_hot[position] = 1
        return one_hot


def collate_fn(batch):
    """
    Custom function to collate data points into batches.
    """
    data, labels = zip(*batch)
    return torch.stack(data), torch.stack(labels)


def get_dataloader(
    extended_grounding_state_list: list[ExtendedGroundingState],
    code_segment_length=5,
    batch_size=1,
    shuffle=True,
    collate_fn=collate_fn,
):
    """
    Custom dataloader to handle sequence data.
    """
    dataset = CodeSegmentDataset(extended_grounding_state_list, code_segment_length)
    if shuffle == True:
        sampler = SequentialSampler(dataset)
        dataloader = DataLoader(
            dataset,
            batch_size=batch_size,
            sampler=sampler,
            collate_fn=collate_fn,
            shuffle=False,
        )
    else:
        dataloader = DataLoader(
            dataset,
            batch_size=batch_size,
            collate_fn=collate_fn,
            shuffle=True,
        )
    return dataloader


class CodeLSTM(nn.Module):
    def __init__(
        self, input_size, hidden_size, output_size, num_layers=4, code_segment_length=5
    ):
        super(CodeLSTM, self).__init__()
        self.hidden_size = hidden_size
        self.num_layers = num_layers
        self.code_segment_length = code_segment_length

        # Define the LSTM layer
        self.lstm = nn.LSTM(input_size, hidden_size, num_layers, batch_first=True)

        self.attention = MultiHeadAttention(hidden_size, heads=heads)

        # Define the output layer
        self.linear = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        # x is already in the shape (batch_size, codeSegmentLength, input_size)

        # Initialize hidden and cell states
        h0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)
        c0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)

        # Forward propagate the LSTM
        out, (final_hidden_state, final_cell_state) = self.lstm(x, (h0, c0))
        out = self.attention(out, out, out, None)
        # Use only the output from the last time step
        out = out[:, -1, :]
        out = self.linear(out)

        return out


################
# run
################
class CustomLossFunction(Function):
    @staticmethod
    def forward(ctx, predictions, labels):
        _, predicted_indices = torch.max(predictions, dim=1)
        _, label_indices = torch.max(labels, dim=1)

        cost = torch.where(
            predicted_indices != label_indices,
            torch.tensor(1.0, device=predictions.device),
            torch.tensor(0.0, device=predictions.device),
        )

        # Save tensors for backward pass
        ctx.save_for_backward(predicted_indices, label_indices, predictions, labels)

        return cost.mean()

    @staticmethod
    def backward(ctx, grad_output):
        # Retrieve tensors from the forward pass
        predicted_indices, label_indices, predictions, labels = ctx.saved_tensors

        # Initialize the gradient tensor with zeros
        grad_predictions = torch.zeros_like(predictions)

        differences = predictions - labels
        # Update the gradient tensor at the predicted indices positions
        for i, (pred_idx, label_idx) in enumerate(
            zip(predicted_indices, label_indices)
        ):
            grad_predictions[i] = 10 * differences[i] * torch.exp(-differences[i])
            grad_predictions[i, label_indices] *= pred_idx != label_idx

        return grad_predictions, None


def evaluate(model, test_dataloader_, criterion):
    model.eval()  # Set the model to evaluation mode
    total_loss = 0.0
    total_correct = 0

    with torch.no_grad():
        for sequences, labels in test_dataloader_:
            sequences = sequences.to(device)
            labels = labels.to(device)

            outputs = model(sequences)

            loss = criterion(outputs, labels)
            total_loss += loss.item()

            # Convert the outputs to probabilities
            probs = torch.sigmoid(outputs)

            # Convert probabilities to binary predictions
            predictions = (probs > 0.5).float()

            total_correct += (predictions == labels).sum().item()

    avg_loss = total_loss / len(test_dataloader_)
    accuracy = total_correct / (
        len(test_dataloader_) * test_dataloader_.batch_size * output_size
    )

    return avg_loss, accuracy


#######
# training and testing
#######
def trainLSTM(trainingDataFileName: str = "trainingData.json", model=None):
    grounding_state_list = initialize_from_file(trainingDataFileName)
    extended_grounding_state_list = [
        ExtendedGroundingState(grounding_state)
        for grounding_state in grounding_state_list
    ]
    # Calculate lengths of splits
    test_extended_grounding_state_list_length = int(
        len(extended_grounding_state_list) * 0.10
    )
    train_extended_grounding_state_list_length: int = (
        len(extended_grounding_state_list) - test_extended_grounding_state_list_length
    )
    # Randomly split dataset into training set and test set
    train_dataset, test_dataset = random_split(
        extended_grounding_state_list,
        [
            train_extended_grounding_state_list_length,
            test_extended_grounding_state_list_length,
        ],
    )

    ######
    ##training part
    ######
    # Load data
    train_dataloader = get_dataloader(
        train_dataset,
        code_segment_length=code_segment_length,
        batch_size=batch_size,
        shuffle=True,
        collate_fn=collate_fn,
    )

    # Instantiate model, loss and optimizer
    if model != None:
        model = CodeLSTM(input_size, hidden_size, output_size, num_layers).to(device)
    criterion = (
        nn.BCEWithLogitsLoss()
        # nn.CrossEntropyLoss()
    )  # This combines a sigmoid layer and the BCE loss, good for binary classification
    optimizer = optim.Adam(model.parameters(), lr=learning_rate)

    for param in model.parameters():
        assert (
            param.requires_grad == True
        ), "param.requires_grad == false in model, exit"

    # Train the model
    model.train()
    loss_function = CustomLossFunction.apply
    # This dictionary will store models based on accuracy
    saved_models = {}
    # The best model details
    best_model = {
        "model_state": None,
        "accuracy": 0.0
    }
    # Lists to store the loss and accuracy values for plotting
    loss_values = []
    accuracy_values = []
    dataLoader_data = []
    for epoch in range(num_epochs):
        epoch_loss = 0.0  # for calculating average loss for the epoch
        epoch_correct = 0  # for calculating average accuracy for the epoch
        total_samples = 0  # total samples in the epoch
        for i, (sequences, labels) in enumerate(train_dataloader):
            data_dict = {
                "epoch": epoch,
                "index": i,
                "sequences": sequences.cpu().numpy().tolist(),
                "labels": labels.cpu().numpy().tolist(),
            }
            dataLoader_data.append(data_dict)

            sequences = sequences.to(device)
            labels = labels.to(device)

            # Forward pass
            outputs = model(sequences)
            mask = (sequences != pad_token).float().to(device)

            # label_mask = (labels != pad_token).float().to(device)
            # labels = labels*label_mask

            # def customLoss(predictions = outputs,labels = labels):
            #     _, predicted_indices = torch.max(predictions, dim=1)
            #     _, label_indices = torch.max(labels, dim=1)

            #     # Create tensors with gradient computation
            #     one_tensor = torch.tensor(1.0, device=predictions.device).requires_grad_()
            #     zero_tensor = torch.tensor(0.0, device=predictions.device).requires_grad_()

            #     # Compute the cost
            #     cost = torch.where(predicted_indices != label_indices, one_tensor, zero_tensor)

            #     return cost # Take the mean if your predictions are in batches

            # loss = criterion(outputs, labels) #built-in loss function
            loss = loss_function(outputs, labels)

            loss = (loss * mask).sum() / mask.sum()

            # Compute accuracy
            _, predicted = torch.max(outputs.data, 1)
            total = labels.size(1)
            correct = (predicted == labels).sum().item()
            accuracy = correct / total

            # Backward pass and optimize
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            if (i + 1) % 10 == 0:
                loss_values.append(loss.item())
                accuracy_values.append(accuracy)
                print(
                    f"Epoch [{epoch+1}/{num_epochs}], Step [{i+1}/{len(train_dataloader)}], Loss: {loss.item():.4f}, Accuracy: {accuracy:.4f}"
                )
            epoch_loss += loss.item()
            epoch_correct += correct
            total_samples += sequences.size(0)

        avg_loss = epoch_loss / len(train_dataloader)
        avg_accuracy = epoch_correct / total_samples
        if epoch % 5 == 0:
            saved_models[avg_accuracy] = {
                "model_state": model.state_dict(),
                "loss": avg_loss,
                "accuracy": avg_accuracy
            }
            if avg_accuracy > best_model["accuracy"]:
                best_model["model_state"] = model.state_dict()
                best_model["accuracy"] = avg_accuracy
    # Set the best model as the final model
    model.load_state_dict(best_model["model_state"])
    # with open("dataloader_data.json", "w") as f:
    #     json.dump(dataLoader_data, f)

    ######
    ##test
    ######
    # test model
    test_dataloader = get_dataloader(
        test_dataset,
        code_segment_length=code_segment_length,
        batch_size=batch_size,
        shuffle=False,
        collate_fn=collate_fn,
    )
    test_loss, test_accuracy = evaluate(model, test_dataloader, criterion)
    print(f"Test Loss: {test_loss:.4f}, Test Accuracy: {test_accuracy:.4f}")

    #### draw curves
    # Plotting Loss
    plt.figure(figsize=(10, 5))
    plt.plot(loss_values, color="red")
    plt.xlabel("Steps (in tens)")
    plt.ylabel("Loss")
    plt.title("Training Loss over Time")
    plt.grid(True)
    plt.show(block=True)

    # Plotting Accuracy
    plt.figure(figsize=(10, 5))
    plt.plot(accuracy_values, color="blue")
    plt.xlabel("Steps (in tens)")
    plt.ylabel("Accuracy")
    plt.title("Training Accuracy over Time")
    plt.grid(True)
    plt.show(block=True)

    return model, test_loss, test_accuracy


class LSTMGuidedReasoning:
    def __init__(self) -> None:
        self.model = CodeLSTM(
            input_size=input_size,
            hidden_size=hidden_size,
            output_size=output_size,
            num_layers=num_layers,
            code_segment_length=code_segment_length,
        ).to(device)
        pass

    def train(self, trainingDataFileName: str = "trainingData.json"):
        trainLSTM(trainingDataFileName=trainingDataFileName, model=self.model)

    def load_model(self, model_path: str):
        self.model.load_state_dict(torch.load(model_path))

    def predict(
        self, grounding_state_str: str, predicted_position_output_file
    ):  # predicted_position_output_file must be an opening file
        eval_extended_grounding_state = ExtendedGroundingState(
            GroundingState(**json.loads(grounding_state_str))
        )
        predicted_result_index = (
            []
        )  # not all outputs are valid, and only these related to input code will be output.
        for (
            extended_code
        ) in eval_extended_grounding_state.extendedCodeTable.extendedCodeList:
            onehot_index = 1  # the begin of output onehot list is flag "succeed", the exact probablity start from index 1
            # Reverse the positionInAST for the described summation
            reversed_ast = extended_code.extendedResult.positionInAST[::-1]

            for i, value in enumerate(reversed_ast, start=1):
                if value == -1:  # left child
                    flag = 0
                elif value == 1:  # right child
                    flag = 1
                else:
                    break  # If value is 0, we break as it's the end signal.

                # Update the onehot value based on the formula
                onehot_index += 2 ** (i - 1 + flag)
            predicted_result_index.append(onehot_index)

        self.model.eval()  # Important! Set the model to evaluation mode
        eval_dataset = [eval_extended_grounding_state]
        eval_dataloader = get_dataloader(
            eval_dataset,
            code_segment_length=code_segment_length,
            batch_size=batch_size,
            shuffle=False,
            collate_fn=collate_fn,
        )

        for sequences, labels in eval_dataloader:  # only loop once
            sequences = sequences.to(device)
            labels = labels.to(device)

            outputs = self.model(sequences)

            # predict_outputs = outputs[0][predicted_result_index]
            predict_outputs = outputs
            probs = torch.sigmoid(predict_outputs)

            # Convert probabilities to binary predictions
            predictions = (probs > 0.5).float()

            print(f"predictions:{predictions.tolist()}\nprobs:{probs.tolist()}")

    def save_model(self, model_path: str):
        torch.save(self.model.state_dict(), model_path)


true = True
false = False

test_grounding_state_json_entry = {
    "codeTable": [
        {
            "grounded": false,
            "operand1": {
                "argName": "x",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
            "operand2": {
                "argName": "2",
                "argType": "number",
                "changeable": 0,
                "className": "",
                "isClass": 0,
            },
            "operator": {"argName": "^", "argType": "other"},
            "result": {
                "argName": "56",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
        },
        {
            "grounded": false,
            "operand1": {
                "argName": "2",
                "argType": "number",
                "changeable": 0,
                "className": "",
                "isClass": 0,
            },
            "operand2": {
                "argName": "56",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
            "operator": {"argName": "*", "argType": "other"},
            "result": {
                "argName": "57",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
        },
        {
            "grounded": false,
            "operand1": {
                "argName": "10",
                "argType": "number",
                "changeable": 0,
                "className": "",
                "isClass": 0,
            },
            "operand2": {
                "argName": "x",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
            "operator": {"argName": "*", "argType": "other"},
            "result": {
                "argName": "58",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
        },
        {
            "grounded": false,
            "operand1": {
                "argName": "57",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
            "operand2": {
                "argName": "58",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
            "operator": {"argName": "+", "argType": "other"},
            "result": {
                "argName": "59",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
        },
        {
            "grounded": false,
            "operand1": {
                "argName": "1",
                "argType": "number",
                "changeable": 0,
                "className": "",
                "isClass": 0,
            },
            "operand2": {
                "argName": "2",
                "argType": "number",
                "changeable": 0,
                "className": "",
                "isClass": 0,
            },
            "operator": {"argName": "+", "argType": "other"},
            "result": {
                "argName": "60",
                "argType": "identifier",
                "changeable": 0,
                "className": "",
                "isClass": 0,
            },
        },
        {
            "grounded": false,
            "operand1": {
                "argName": "59",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
            "operand2": {
                "argName": "60",
                "argType": "identifier",
                "changeable": 0,
                "className": "",
                "isClass": 0,
            },
            "operator": {"argName": "+", "argType": "other"},
            "result": {
                "argName": "61",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
        },
        {
            "grounded": false,
            "operand1": {
                "argName": "61",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
            "operand2": {
                "argName": "0",
                "argType": "number",
                "changeable": 0,
                "className": "",
                "isClass": 0,
            },
            "operator": {"argName": "==", "argType": "other"},
            "result": {
                "argName": "62",
                "argType": "identifier",
                "changeable": 1,
                "className": "",
                "isClass": 0,
            },
        },
    ],
    "energy": 0.7999999964776927,
    "initialState": false,
    "position": 6,
    "relatedFilesAndClasses": ["assemblyCode#0 "],
    "succeed": true,
}
# lstm_model,ave_loss,accuracy = trainLSTM()
# torch.save(lstm_model.state_dict(), 'model_checkpoint.txt')

# model = CodeLSTM(input_size, hidden_size, output_size, num_layers).to(device)  # Initialize the model architecture, this step costs lots of time
# model.load_state_dict(torch.load('model_checkpoint.txt'))

predict_output_file = open("predict_result.txt", "w")
model_handler = LSTMGuidedReasoning()
model_handler.train("trainingData.json")
model_handler.save_model("model_checkpoint.mdl")
model_handler.load_model("model_checkpoint.mdl")
model_handler.predict(json.dumps(test_grounding_state_json_entry), predict_output_file)
# with open("code_segment_embedding_data.json", "w") as f:
#     json.dump(code_segment_embedding_data, f, indent=4)
