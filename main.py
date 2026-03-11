import math

# https://www.geeksforgeeks.org/python/how-to-read-from-a-file-in-python/
# https://www.reddit.com/r/learnpython/comments/ynzzum/how_to_convert_scientific_notation_string_to/
def data_processing(file_name):
    data = []
    with open(file_name, "r") as f:
        for line in f:
            row = line.split()
            for i in range(len(row)):
                row[i] = float(row[i])
            data.append(row)
    return data

# https://www.geeksforgeeks.org/maths/euclidean-distance/
def calculate_distance(vector1, vector2):
    num = 0
    for i in range(len(vector1)):
        num += (vector2[i] - vector1[i]) ** 2

    num = math.sqrt(num)
    return num

def leave_one_out_cross_validation_forward(data, current_features, feature_to_add):
    num_correctly_classified = 0
    features_to_use = current_features.copy()
    features_to_use.append(feature_to_add)

    for i in range(len(data)):
        object_to_classify = []
        for feature in features_to_use:
            object_to_classify.append(data[i][feature])
        label_object_to_classify = data[i][0]    
        nearest_neighbor_distance = math.inf
        nearest_neighbor_location = math.inf
        for j in range(len(data)):
            if j != i:
                neighbor = []
                for feature in features_to_use:
                    neighbor.append(data[j][feature])
                distance = calculate_distance(object_to_classify, neighbor)
                if distance < nearest_neighbor_distance:
                    nearest_neighbor_distance = distance
                    nearest_neighbor_location = j
                    nearest_neighbor_label = data[nearest_neighbor_location][0]

        if label_object_to_classify == nearest_neighbor_label:
            num_correctly_classified += 1

    accuracy = num_correctly_classified / len(data)
    print(f"Using feature(s) {features_to_use} accuracy is {accuracy}")
    return accuracy

def forward_selection(data):
    current_set_of_features = []
    best_accuracies = []

    for i in range(len(data[0])-1):
        feature_to_add_at_this_level = []
        best_so_far_accuracy = 0

        for j in range(1, len(data[0])):
            if j not in current_set_of_features:
                print(f"--Considering adding the {j} feature")
                accuracy = leave_one_out_cross_validation_forward(data, current_set_of_features, j)
                if accuracy > best_so_far_accuracy:
                    best_so_far_accuracy = accuracy
                    feature_to_add_at_this_level = j
        
        current_set_of_features.append(feature_to_add_at_this_level)
        best_accuracies.append([current_set_of_features.copy(), best_so_far_accuracy])
        print("")
        if len(best_accuracies) > 1:
            if best_so_far_accuracy < best_accuracies[i-1][1]:
                print("(Warning, Accuracy has decreased. Continuing search incase of local maxima)")
        print(f"Feature set {current_set_of_features} was best, accuracy is {best_so_far_accuracy}\n")
    best_accuracies.sort(key=lambda x: x[1], reverse=True)
    print("Finished search")
    print(f"Highest accuracy is {best_accuracies[0][1]} using features {best_accuracies[0][0]}")

def leave_one_out_cross_validation_backward(data, current_features, feature_to_remove=None):
    num_correctly_classified = 0
    features_to_use = current_features.copy()
    if feature_to_remove:
        features_to_use.remove(feature_to_remove)

    for i in range(len(data)):
        object_to_classify = []
        for feature in features_to_use:
            object_to_classify.append(data[i][feature])
        label_object_to_classify = data[i][0]    
        nearest_neighbor_distance = math.inf
        nearest_neighbor_location = math.inf
        for j in range(len(data)):
            if j != i:
                neighbor = []
                for feature in features_to_use:
                    neighbor.append(data[j][feature])
                distance = calculate_distance(object_to_classify, neighbor)
                if distance < nearest_neighbor_distance:
                    nearest_neighbor_distance = distance
                    nearest_neighbor_location = j
                    nearest_neighbor_label = data[nearest_neighbor_location][0]

        if label_object_to_classify == nearest_neighbor_label:
            num_correctly_classified += 1

    accuracy = num_correctly_classified / len(data)
    print(f"Using feature(s) {features_to_use} accuracy is {accuracy}")
    return accuracy

def backward_elimination(data):
    current_set_of_features = []
    for i in range(1, len(data[0])):
        current_set_of_features.append(i)
    accuracy = leave_one_out_cross_validation_backward(data, current_set_of_features)
    print("")
    best_accuracies = [[current_set_of_features.copy(), accuracy]]
    for i in range(len(data[0])-2):
        feature_to_remove_at_this_level = []
        best_so_far_accuracy = 0

        for j in range(1, len(data[0])):
            if j in current_set_of_features:
                print(f"--Considering removing feature {j}")
                accuracy = leave_one_out_cross_validation_backward(data, current_set_of_features, j)
                if accuracy > best_so_far_accuracy:
                    best_so_far_accuracy = accuracy
                    feature_to_remove_at_this_level = j
                
        current_set_of_features.remove(feature_to_remove_at_this_level)
        best_accuracies.append([current_set_of_features.copy(), best_so_far_accuracy])
        print("")
        if len(best_accuracies) > 1:
            if best_so_far_accuracy < best_accuracies[i-1][1]:
                print("(Warning, Accuracy has decreased. Continuing search incase of local maxima)")
        print(f"Feature set {current_set_of_features} was best, accuracy is {best_so_far_accuracy}\n")
    best_accuracies.sort(key=lambda x: x[1], reverse=True)
    print("Finished Search")
    print(f"Highest accuracy is {best_accuracies[0][1]} using features {best_accuracies[0][0]}")
    return

def test_algorithim():
    file_name = input("Type in the name of file to test: ")
    data = data_processing(file_name)
    num_features = len(data[0])
    print("Type the number of the algorithim you want to run\n")
    print("1) Forward Selection")
    print("2} Backward Elimination\n")
    user_input = input()
    while not (user_input == "1" or user_input == "2"):
        print("Unrecognized input, try again")
        user_input = input()

    print(f"\nThis data set has {num_features-1} features (not including the class attribute), with {len(data)} instances.\n")
    
    if user_input == "1":
        forward_selection(data)

    else:
        backward_elimination(data)

test_algorithim()