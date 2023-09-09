import json
from collections import defaultdict

import inspect
def current_pos():
    print("file:///"+__file__+'#'+str(inspect.currentframe().f_back.f_lineno))

###### Extract GroundingState entries from json file.       

# Defining the classes
class Operand:
    def __init__(self, argName="", argType="", changeable=False, className="", isClass=False):
        self.argName = argName
        self.argType = argType
        self.changeable = changeable
        self.className = className
        self.isClass = isClass

class Operator:
    def __init__(self, argName="", argType=""):
        self.argName = argName
        self.argType = argType

class Result(Operand):
    pass

class Code:
    def __init__(self, grounded=False, operand1={}, operand2={}, operator={}, result={}):
        self.grounded = grounded
        self.operand1 = Operand(**operand1)
        self.operand2 = Operand(**operand2)
        self.operator = Operator(**operator)
        self.result = Result(**result)

class CodeTable:
    def __init__(self, codeTable):
        # If codeTable is a CodeTable instance, extract the actual list of codeList
        if isinstance(codeTable, CodeTable):
            self.codeList = codeTable.codeList
        else:
            self.codeList = [Code(**code) for code in codeTable]

class GroundingState:
    def __init__(self, codeTable=[], energy=0.0, initialState=False, position=0, relatedFilesAndClasses=[], succeed=False):
        self.codeTable = CodeTable(codeTable)
        self.energy = energy
        self.initialState = initialState
        self.position = position
        self.relatedFilesAndClasses = relatedFilesAndClasses
        self.succeed = succeed

# Function to initialize GroundingState from a JSON file
def initialize_from_file(filename):
    with open(filename, 'r') as file:
        data = json.load(file)
    # return [GroundingState(**entry) for entry in data]
    groundingStateList = []
    for i,entry in enumerate(data):
    #    print("iternum:"+str(i)+"\nentry:\n"+str(entry))
        groundingStateInstance = GroundingState(**entry)
        groundingStateList.append(groundingStateInstance)
    return groundingStateList




# import json
# from collections import defaultdict

# Base classes
# ... [All previously defined classes here, such as Operand, Code, etc.] ...

# Extended classes
class ExtendedOperand(Operand):

    def __init__(self, argName, argType, changeable=False, className="", isClass="0"):
        super().__init__(argName, argType, changeable, className, isClass)
        self.occurrenceNumber = 0
        self.isLeafNode = False
        self.positionInAST = []
        self.argValue = None  # New member for storing numeric values

        # Check if argType is 'number' and handle the argName and argValue assignment
        if self.argType == "number":
            self.argValue = float(self.argName)
            self.argName = None

    # Override the equality operator
    def __eq__(self, other):
        if isinstance(other, ExtendedOperand):
            return (self.argName == other.argName and 
                    self.argType == other.argType and
                    self.changeable == other.changeable and
                    self.className == other.className and
                    self.isClass == other.isClass and 
                    self.argValue == other.argValue)
        return False

    # It's good practice to override the not equal operator when overriding the equality operator
    def __ne__(self, other):
        return not self.__eq__(other)

    # Implementing the less than operator
    def __lt__(self, other):
        if not isinstance(other, ExtendedOperand):
            return NotImplemented
        # If both operands have argValue, compare based on it
        if self.argValue is not None and other.argValue is not None:
            return self.argValue < other.argValue

        # If only one operand has argValue, the one with it is considered "smaller"
        if self.argValue is not None:
            return True
        if other.argValue is not None:
            return False        
        # Primary sorting criterion: argName
        if self.argName != other.argName:
            return self.argName < other.argName
        # Secondary sorting criterion: argType
        return self.argType < other.argType


class ExtendedOperator(Operator):
    # Add any extra attributes or methods as needed
    pass

class ExtendedResult(ExtendedOperand):
    pass

class ExtendedCode:
    def __init__(self, code:Code):
        self.grounded = code.grounded
        self.extendedOperand1 = ExtendedOperand(**code.operand1.__dict__)
        self.extendedOperand2 = ExtendedOperand(**code.operand2.__dict__)
        self.extendedOperator = ExtendedOperator(**code.operator.__dict__)
        self.extendedResult = ExtendedResult(**code.result.__dict__)

    def get_positionInAST(self):
        return self.extendedResult.positionInAST

class ExtendedCodeTable:
    def __init__(self, codeTable:CodeTable):
        # super().__init__(*args, **kwargs)
        self.extendedCodeList = [ExtendedCode(code) for code in codeTable.codeList]
        self.identifierSubstitutionTable = defaultdict(list)
        self.uniqueIdentifiers = set()
        self.nodeLocalize()
        self.identifierSubstitute()
        self.updateUniqueIdentifiers()
        self.updateOccurrenceNumber()

    def nodeLocalize(self):
        parentNodeSet = set()
        processedNodeSet = {} # Now a dictionary mapping node name to its position

        rootNode = self.extendedCodeList[-1].extendedResult
        rootNode.positionInAST = [0]
        parentNodeSet.add(rootNode.argName)
        processedNodeSet[rootNode.argName] = rootNode.positionInAST

        # Helper function updated to retrieve position from processedNodeSet
        def find_position(arg_name):
            """Helper function to find previously processed positionInAST"""
            return processedNodeSet.get(arg_name)

        # Traverse the list of codeList in reverse
        for extendedCode in reversed(self.extendedCodeList[:-1]):

            # Check and assign attributes for extendedOperand1
            if extendedCode.extendedOperand1.argName in parentNodeSet:
                extendedCode.extendedOperand1.positionInAST = find_position(extendedCode.extendedOperand1.argName) or extendedCode.extendedResult.positionInAST + [-1]
                extendedCode.extendedOperand1.isLeafNode = False
            elif extendedCode.extendedOperand1.argName not in processedNodeSet:
                extendedCode.extendedOperand1.positionInAST = find_position(extendedCode.extendedOperand1.argName) or extendedCode.extendedResult.positionInAST + [-1]
                extendedCode.extendedOperand1.isLeafNode = True
                processedNodeSet[extendedCode.extendedOperand1.argName] = extendedCode.extendedOperand1.positionInAST

            # Check and assign attributes for extendedOperand2
            if extendedCode.extendedOperand2.argName in parentNodeSet:
                extendedCode.extendedOperand2.positionInAST = find_position(extendedCode.extendedOperand2.argName) or extendedCode.extendedResult.positionInAST + [1]
                extendedCode.extendedOperand2.isLeafNode = False
            elif extendedCode.extendedOperand2.argName not in processedNodeSet:
                extendedCode.extendedOperand2.positionInAST = find_position(extendedCode.extendedOperand2.argName) or extendedCode.extendedResult.positionInAST + [1]
                extendedCode.extendedOperand2.isLeafNode = True
                processedNodeSet[extendedCode.extendedOperand2.argName] = extendedCode.extendedOperand2.positionInAST

            parentNodeSet.add(extendedCode.extendedResult.argName)
            processedNodeSet[extendedCode.extendedResult.argName] = extendedCode.extendedResult.positionInAST



    def identifierSubstitute(self):
        # Stub: Implement identifier substitution logic here using identifierSubstitutionTable
        for extendedCode in self.extendedCodeList:

            # Check and replace for extendedOperand1
            if not extendedCode.extendedOperand1.isLeafNode and extendedCode.extendedOperand1.argType == "identifier":
                newPath = ''.join(map(str, extendedCode.extendedOperand1.positionInAST)).replace('-1', 'L').replace('1', 'R')
                extendedCode.extendedOperand1.argName = "var" + newPath

            # Check and replace for extendedOperand2
            if not extendedCode.extendedOperand2.isLeafNode and extendedCode.extendedOperand2.argType == "identifier":
                newPath = ''.join(map(str, extendedCode.extendedOperand2.positionInAST)).replace('-1', 'L').replace('1', 'R')
                extendedCode.extendedOperand2.argName = "var" + newPath

    def updateUniqueIdentifiers(self):
        # Reset the set
        self.uniqueIdentifiers = set()
        for extendedCode in self.extendedCodeList:
            # Check and add for extendedOperand1
            if extendedCode.extendedOperand1.argType == "identifier":
                self.uniqueIdentifiers.add(extendedCode.extendedOperand1.argName)
                
            # Check and add for extendedOperand2
            if extendedCode.extendedOperand2.argType == "identifier":
                self.uniqueIdentifiers.add(extendedCode.extendedOperand2.argName)
                
            # Check and add for extendedResult
            if hasattr(extendedCode, 'extendedResult') and extendedCode.extendedResult.argType == "identifier":
                self.uniqueIdentifiers.add(extendedCode.extendedResult.argName)

    def updateOccurrenceNumber(self):
        operand_count_dict = {}

        # First pass: Count occurrences
        for extendedCode in self.extendedCodeList:
            # Check and count for extendedOperand1
            if extendedCode.extendedOperand1.argName not in operand_count_dict:
                operand_count_dict[extendedCode.extendedOperand1.argName] = 0
            operand_count_dict[extendedCode.extendedOperand1.argName] += 1
            
            # Check and count for extendedOperand2
            if extendedCode.extendedOperand2.argName not in operand_count_dict:
                operand_count_dict[extendedCode.extendedOperand2.argName] = 0
            operand_count_dict[extendedCode.extendedOperand2.argName] += 1
        
        # Second pass: Assign occurrenceNumber to ExtendedOperands
        for extendedCode in self.extendedCodeList:
            extendedCode.extendedOperand1.occurrenceNumber = operand_count_dict.get(extendedCode.extendedOperand1.argName, 0)
            extendedCode.extendedOperand2.occurrenceNumber = operand_count_dict.get(extendedCode.extendedOperand2.argName, 0)


class ExtendedGroundingState:
    def __init__(self, groundingState:GroundingState):
        # Assuming the grounding_state is an instance of GroundingState
        # super().__init__(
        #     codeTable=groundingState.codeTable, 
        #     energy=groundingState.energy, 
        #     initialState=groundingState.initialState, 
        #     position=groundingState.position, 
        #     relatedFilesAndClasses=groundingState.relatedFilesAndClasses, 
        #     succeed=groundingState.succeed
        # )
        self.extendedCodeTable=ExtendedCodeTable(groundingState.codeTable)
        self.energy=groundingState.energy
        self.initialState=groundingState.initialState 
        self.position=groundingState.position 
        self.relatedFilesAndClasses=groundingState.relatedFilesAndClasses
        self.succeed=groundingState.succeed
        self.oneshotPosition = self.getOneshotPosition()
    def getOneshotPosition(self):
        # Step 1: Find the code corresponding to ExtendedGroundingState.position in codeTable.
        extendedCode = self.extendedCodeTable.extendedCodeList[self.position]

        # Step 2: Calculate the oneshotPosition using code.result.positionInAST
        oneshot_val = 0

        # Reverse the positionInAST for the described summation
        reversed_ast = extendedCode.extendedResult.positionInAST[::-1]
        
        for i, value in enumerate(reversed_ast, start=1):
            if value == -1: # left child
                flag = 0
            elif value == 1: # right child
                flag = 1
            else:
                break  # If value is 0, we break as it's the end signal.

            # Update the oneshot value based on the formula
            oneshot_val += (2 ** (i - 1 + flag))
        
        return oneshot_val
        

##### main
# grounding_states = initialize_from_file("trainingData.json")
# extendedGroundingState = ExtendedGroundingState(grounding_states[0])
# print(extendedGroundingState.__dict__,4)


# def getOneshotPosition(position):
#         # Step 1: Find the code corresponding to ExtendedGroundingState.position in codeTable.


#         # Step 2: Calculate the oneshotPosition using code.result.positionInAST
#         oneshot_val = 0

#         # Reverse the positionInAST for the described summation
#         reversed_ast = position[::-1]
        
#         for i, value in enumerate(reversed_ast, start=1):
#             if value == -1: # left child
#                 flag = 0
#             elif value == 1: # right child
#                 flag = 1
#             else:
#                 break  # If value is 0, we break as it's the end signal.

#             # Update the oneshot value based on the formula
#             oneshot_val += (2 ** (i - 1 + flag))
        
#         return oneshot_val

# print(getOneshotPosition([1,1,1,1,1,1,1,1]))


