# trainingProcess.py
from extractTrainingDataFromJsonFile import *
from embeddingTrainingData import *

# for training
import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader, SequentialSampler
import torch.optim as optim

# for testing
from torch.utils.data.dataset import random_split
from torch.utils.data import DataLoader

# Define the device here for global accessibility
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")


class CodeSegmentDataset(Dataset):
    def __init__(
        self,
        extended_grounding_state_list: list[ExtendedGroundingState],
        code_segment_length=5,
    ):
        self.data: list[ExtendedGroundingState] = extended_grounding_state_list
        self.code_segment_length = code_segment_length

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        extended_grounding_state = self.data[idx]
        extended_code_table = extended_grounding_state.extendedCodeTable
        extended_code_embeddings = [
            torch.tensor(extended_code_embedding(extended_code))
            for extended_code in extended_code_table.extendedCodeList[::-1]
        ]

        # Make sure every input is of fixed length by padding or truncating
        extended_code_segments = []
        if len(extended_code_embeddings) <= self.code_segment_length:
            segment_tensor = extended_code_embeddings
            while (
                len(segment_tensor) < self.code_segment_length
            ):  # Just to ensure that the last segment has the required length, though it might not be necessary
                segment_tensor.append(torch.zeros_like(segment_tensor[0]))
            extended_code_segments = torch.cat(segment_tensor, dim=0)
            extended_code_segments = torch.unsqueeze(extended_code_segments, dim=0)
        else:
            for i in range(
                0, len(extended_code_embeddings) - self.code_segment_length + 1
            ):
                segment_tensor = extended_code_embeddings[
                    i : i + self.code_segment_length
                ]
                # while (
                #     len(segment) < self.code_segment_length
                # ):  # Just to ensure that the last segment has the required length, though it might not be necessary
                #     segment.append(torch.zeros_like(segment[0]))
                segment_tensor = torch.cat(segment_tensor, dim=0)
                extended_code_segments.append(segment_tensor)
            extended_code_segments = torch.stack(extended_code_segments, dim=0)

        # Convert list of tensors to a single tensor with an extra sequence length dimension
        # extended_code_embeddings_tensor = torch.stack(extended_code_embeddings)

        # Your SOP mentioned combining labels for output, so we concatenate accordingly.
        label = torch.cat(
            [
                torch.tensor(
                    [1 - extended_grounding_state.succeed], dtype=torch.float32
                ),
                self._one_hot_encode(extended_grounding_state.oneshotPosition, 511),
            ]
        )

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
        self, input_size, hidden_size, output_size, num_layers=1, code_segment_length=5
    ):
        super(CodeLSTM, self).__init__()
        self.hidden_size = hidden_size
        self.num_layers = num_layers
        self.code_segment_length = code_segment_length

        # Define the LSTM layer
        self.lstm = nn.LSTM(input_size, hidden_size, num_layers, batch_first=True)

        # Define the output layer
        self.linear = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        # x is already in the shape (batch_size, codeSegmentLength, input_size)

        # Initialize hidden and cell states
        h0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)
        c0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)

        # Forward propagate the LSTM
        out, _ = self.lstm(x, (h0, c0))

        # Use only the output from the last time step
        out = out[:, -1, :]
        out = self.linear(out)

        return out


################
# run
################
grounding_state_list = initialize_from_file("trainingData.json")
extended_grounding_state_list = [
    ExtendedGroundingState(grounding_state) for grounding_state in grounding_state_list
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

CODE_EMBEDDING_SIZE = int(
    extended_code_embedding(
        extended_grounding_state_list[0].extendedCodeTable.extendedCodeList[0]
    ).__len__()
)

# Hyperparameters
input_size = 5 * (
    CODE_EMBEDDING_SIZE
)  # Assuming each code embedding has a size of YOUR_EMBEDDING_SIZE and codeSegmentLength=5
hidden_size = 128
output_size = 512
num_layers = 1
num_epochs = 10
batch_size = 1
learning_rate = 0.001

######
##training part
######
# Load data
train_dataloader = get_dataloader(
    train_dataset,
    code_segment_length=5,
    batch_size=batch_size,
    shuffle=True,
    collate_fn=collate_fn,
)

# Instantiate model, loss and optimizer
model = CodeLSTM(input_size, hidden_size, output_size, num_layers).to(device)
criterion = (
    nn.BCEWithLogitsLoss()
)  # This combines a sigmoid layer and the BCE loss, good for binary classification
optimizer = optim.Adam(model.parameters(), lr=learning_rate)

# Train the model
model.train()
for epoch in range(num_epochs):
    for i, (sequences, labels) in enumerate(train_dataloader):
        sequences = sequences.to(device)
        labels = labels.to(device)

        # Forward pass
        outputs = model(sequences)
        loss = criterion(outputs, labels)

        # Backward pass and optimize
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        if (i + 1) % 10 == 0:
            print(
                f"Epoch [{epoch+1}/{num_epochs}], Step [{i+1}/{len(train_dataloader)}], Loss: {loss.item():.4f}"
            )

######
##test
######
# test model
test_dataloader = get_dataloader(
    test_dataset,
    code_segment_length=5,
    batch_size=batch_size,
    shuffle=False,
    collate_fn=collate_fn,
)


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


test_loss, test_accuracy = evaluate(model, test_dataloader, criterion)
print(f"Test Loss: {test_loss:.4f}, Test Accuracy: {test_accuracy:.4f}")
