data = []
# https://www.geeksforgeeks.org/python/how-to-read-from-a-file-in-python/
# https://www.reddit.com/r/learnpython/comments/ynzzum/how_to_convert_scientific_notation_string_to/
def data_processing(file_name):
    with open(file_name, "r") as f:
        for line in f:
            row = line.split()
            for i in range(len(row)):
                row[i] = float(row[i])
            data.append(row)

def nearest_neighbor(data):
    return

def forward_selection(data):
    return

def backward_elimination(data):
    return

data_processing("SanityCheck_DataSet__1.txt")
print(data[0])