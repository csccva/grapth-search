import random
import string

def generate_dependencies():
    alphabet = string.ascii_uppercase  # List of uppercase letters A-Z
    dependencies = []  # List to store the dependency strings

    # Loop through each letter in the alphabet
    for left in alphabet:
        # Randomly choose the number of dependencies (0, 1, 2, 3, or 4)
        num_dependencies = random.choices(
            [0, 1, 2, 3, 4],  # Possible numbers of dependencies
            weights=[54, 25, 12.5, 6.5, 2],  # Corresponding probabilities (weights)
            k=1  # Only pick one number for each letter
        )[0]
        
        # If there are dependencies (num_dependencies > 0)
        if num_dependencies > 0:
            # Pick 'num_dependencies' different letters from the alphabet excluding 'left'
            right_choices = random.sample([letter for letter in alphabet if letter != left], num_dependencies)
            # Add each dependency to the list with " -> " format
            for right in right_choices:
                dependencies.append(f"{left} -> {right}")
    
    return dependencies

def save_dependencies_to_file(filename="dependencies.txt"):
    dependencies = generate_dependencies()  # Generate the dependencies
    # Open the file and write the dependencies to it
    with open(filename, "w") as file:
        for dependency in dependencies:
            file.write(dependency + "\n")  # Write each dependency on a new line

# Generate and save dependencies to a file
save_dependencies_to_file()
