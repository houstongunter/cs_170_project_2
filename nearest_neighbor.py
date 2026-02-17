import math

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

# https://www.geeksforgeeks.org/maths/euclidean-distance/
def calculate_distance(vector1, vector2):
    num = 0
    for i in range(len(vector1)):
        num += (vector2[i] - vector1[i]) ** 2

    num = math.sqrt(num)
    return num

def nearest_neighbor(test_data_row: int, features: list[int]):
    test_vector = []
    min_distance = None
    # grab the correct features for the point being tested
    for feature in features:
        test_vector.append(data[test_data_row][feature])
    for i in range(len(data)):
        # make sure to not compare to itself
        if i == test_data_row:
            continue
        test_vector2 = []
        # grab the correct features for the point in the data set
        for feature in features:
            test_vector2.append(data[i][feature])
        dist = calculate_distance(test_vector, test_vector2)
        # replace min_distance if very first point being compared
        if min_distance == None:
            min_distance = (data[i][0], dist)
        # if not only replace min_distance if it is closer
        elif dist < min_distance[1]:
            min_distance = (data[i][0], dist)
    return min_distance[0]

def forward_selection(data):
    return

def backward_elimination(data):
    return

data_processing("SanityCheck_DataSet__1.txt")
num_features = len(data[0] - 1)
row_class = int(data[0][0])

def kfold_cross_validation():
    return