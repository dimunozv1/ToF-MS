def multiply_numbers_by_constant(numbers, constant):
    return [num * constant for num in numbers]

def compute_time_difference(numbers):
    time_difference = []
    for i in range(len(numbers) - 1):
        diff = numbers[i+1] - numbers[i]
        time_difference.append(diff)
    return time_difference

if __name__ == "__main__":
    # Name of the input .txt file containing numbers separated by newlines
    input_file_name = "25000Hz.txt"

    # Constant value (500e-06 or 0.0005)
    constant_value = 500e-06

    try:
        # Read the numbers from the input file
        with open(input_file_name, "r") as file:
          
            input_numbers = [float(line.strip()) for line in file]

        # Multiply the numbers by the constant
        result = multiply_numbers_by_constant(input_numbers, constant_value)
        time_difference = compute_time_difference(result)
        # Print the resulting values
        print("Input Numbers:", input_numbers)
        print("Constant Value:", constant_value)
        print("Result after Multiplication:", result)
        print("Time difference:", time_difference)

    except FileNotFoundError:
        print(f"Error: File '{input_file_name}' not found.")
    except ValueError:
        print(f"Error: Invalid format in '{input_file_name}'. Please make sure the file contains numbers separated by newlines.")
