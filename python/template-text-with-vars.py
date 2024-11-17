# Define the User class
class User:
    def __init__(self, name, age, country):
        self.name = name
        self.age = age
        self.country = country

# Define a class to handle missing keys by returning the placeholder itself
class SafeDict(dict):
    def __missing__(self, key):
        return f'{{{key}}}'  # Return the placeholder as {key} if the key is missing

# Function to convert a User object to a SafeDict
def user_to_safe_dict(user: User) -> SafeDict:
    return SafeDict({
        "var_user_name": user.name,
        "var_age": user.age,
        "var_country": user.country
    })

# Function to replace placeholders
def generate_text(template: str, dict_vars: SafeDict) -> str:
    # Use str.format_map to substitute placeholders
    return template.format_map(dict_vars)

# Example usage
template = """I am {var_user_name}. I'm {var_age} years old.
I am from {var_country}.
I am {var_user_name}. This is '{var_undefined}'.
I am {var_user_name}. This is {var_undefined}."""

user = User(name="John", age=30, country="USA")

dict_vars = user_to_safe_dict(user)
output = generate_text(template, dict_vars)

print(output)



"""
I am John. I'm 30 years old.
I am from USA.
I am John. This is '{var_undefined}'.
I am John. This is {var_undefined}.
"""