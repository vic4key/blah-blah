import re

# Define the User class
class User:
    def __init__(self, name, age, country):
        self.name = name
        self.age = age
        self.country = country

# Define a function to replace placeholders
def generate_text(user: User, template: str) -> str:
    # Create a dictionary of user attributes to use in substitution
    data = {
        "var_user_name": user.name,
        "var_age": user.age,
        "var_country": user.country
    }
    
    # Function to replace placeholders
    def replace(match):
        key = match.group(1)  # Extract the key inside { }
        # If key exists in data, convert it to string, otherwise keep the placeholder
        return str(data.get(key, f'{{{key}}}'))
    
    # Use regular expressions to find placeholders and replace them
    return re.sub(r'\{(\w+)\}', replace, template)

# Example usage
template = """I am {var_user_name}. I'm {var_age} years old.
I am from {var_country}.
I am {var_user_name}. This is {var_undefined}.
I am {var_user_name}. This is {var_undefined}."""

user = User(name="John", age=30, country="USA")
output = generate_text(user, template)

print(output)



"""
I am John. I'm 30 years old.
I am from USA.
I am John. This is {var_undefined}.
I am John. This is {var_undefined}.
"""