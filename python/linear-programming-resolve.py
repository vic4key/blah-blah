# https://www.geeksforgeeks.org/python-linear-programming-in-pulp/

# import the library pulp as p
import pulp as lp

# Create a LP Minimization problem
LP_prob = lp.LpProblem("Problem", lp.LpMinimize)

# Create problem Variables
x = lp.LpVariable("x", lowBound = 0) # Create a variable x >= 0
y = lp.LpVariable("y", lowBound = 0) # Create a variable y >= 0

# Objective Function
LP_prob += 3 * x + 5 * y

# Constraints:
LP_prob += 2 * x + 3 * y >= 12
LP_prob += -x + y <= 3
LP_prob += x >= 4
LP_prob += y <= 3

# Display the problem
print(LP_prob)  

status = LP_prob.solve() # Solver
print(lp.LpStatus[status]) # The solution status

# Printing the final solution
print(lp.value(x), lp.value(y), lp.value(LP_prob.objective))
