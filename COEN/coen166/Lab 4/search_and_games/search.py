# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util
import sys
import copy

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def goalTest(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getActions(self, state):
        """
        Given a state, returns available actions.
        Returns a list of actions
        """        
        util.raiseNotDefined()

    def getResult(self, state, action):
        """
        Given a state and an action, returns resulting state.
        """
        util.raiseNotDefined()

    def getCost(self, state, action):
        """
        Given a state and an action, returns step cost, which is the incremental cost 
        of moving to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()

class Node:
    """
    Search node object for your convenience.

    This object uses the state of the node to compare equality and for its hash function,
    so you can use it in things like sets and priority queues if you want those structures
    to use the state for comparison.

    Example usage:
    >>> S = Node("Start", None, None, 0)
    >>> A1 = Node("A", S, "Up", 4)
    >>> B1 = Node("B", S, "Down", 3)
    >>> B2 = Node("B", A1, "Left", 6)
    >>> B1 == B2
    True
    >>> A1 == B2
    False
    >>> node_list1 = [B1, B2]
    >>> B1 in node_list1
    True
    >>> A1 in node_list1
    False
    """
    def __init__(self, state, parent, action, path_cost):
        self.state = state
        self.parent = parent
        self.action = action
        self.path_cost = path_cost

    def __hash__(self):
        return hash(self.state)

    def __eq__(self, other):
        return self.state == other.state

    def __ne__(self, other):
        return self.state != other.state


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def breadthFirstSearch(problem):
    #states to be explored(FIFO) queue
    frontier = util.Queue() #frontier queue
    visited = [] #visited nodes
    startState = problem.getStartState()
    frontier.push([startState, []]) #pushes the state and list of actions

    while frontier.isEmpty() != True:
        curState = frontier.pop() # curState[0] is the state, curState[1] is the list of actions taken
        
        if curState[0] not in visited: #if not visited before, append state to visited
            visited += curState[0]

            if problem.goalTest(curState[0]) == True: #if at goal state return the actions
                return curState[1]
            else:
                suc = problem.getActions(curState[0]) #get a list of available actions
                if (len(suc) != 0): #makes sure there are available actions
                    for path in suc:
                        flag = True #flag for checking the queue
                        nextState = problem.getResult(curState[0], path) #gets the next state
                        nextPath = curState[1] + [path] #appends the path to actions
                        if (nextState not in visited): #makes sure that the next state hasn't been visited
                            for i in frontier.list: #makes sure that the next state isn't in the queue
                                if nextState == i[0]: #if the state is in the frontier, then don't push it to the frontier
                                    flag = False
                        if flag == True:
                            frontier.push([nextState, nextPath]) #pushes the state and actions to frontier
    
def depthFirstSearch(problem): 
    #states to be explore LIFO stack
    frontier = util.Stack() #frontier set
    visited = [] #visited states
    startState = problem.getStartState() 
    frontier.push([startState, []]) #push the start state and no actions

    while not frontier.isEmpty():
        curState = frontier.pop() #curState[0] = state, curState[1] = list of actions
        if curState[0] not in visited: #if the state has not been visited, append 
            visited.append(curState[0])

            if problem.goalTest(curState[0]) == True: #if state is the goal state, return actions
                return curState[1]
            else:
                suc = problem.getActions(curState[0]) #list of possible actions
                if (len(suc) != 0): #makes sure that there are available actions
                    for path in suc:
                        flag = True
                        nextState = problem.getResult(curState[0], path) #next state = current state + action
                        nextPath = curState[1] + [path] #next path = current actions + next action
                        if (nextState not in visited):
                            for i in frontier.list:
                                if nextState == i[0]:
                                    flag = False
                        if flag == True: 
                            frontier.push([nextState, nextPath]) #push the next state and path into the frontier set

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def iterativeDeepeningSearch(problem):
    """
    Perform DFS with increasingly larger depth. Begin with a depth of 1 and increment depth by 1 at every step.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.goalTest(problem.getStartState()))
    print("Actions from start state:", problem.getActions(problem.getStartState()))

    Then try to print the resulting state for one of those actions
    by calling problem.getResult(problem.getStartState(), one_of_the_actions)
    or the resulting cost for one of these actions
    by calling problem.getCost(problem.getStartState(), one_of_the_actions)

    """
    "*** YOUR CODE HERE ***"
    util.raiseNotDefined()
    
def UniformCostSearch(problem):
    """Search the node that has the lowest path cost first."""
    "*** YOUR CODE HERE ***"  
    util.raiseNotDefined()
    

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    frontier = util.PriorityQueue()
    visited = []
    frontier.push((problem.getStartState(), []), 0) #(state, actions), total cost
    while (frontier.isEmpty() != True):
        curState = frontier.pop()
        if problem.goalTest(curState[0]) == True:
            return curState[1]
        if curState[0] not in visited:
            visited.append(curState[0])

        for path in problem.getActions(curState[0]):
            nextState = problem.getResult(curState[0], path)
            
            if nextState not in visited:
                nextPath = curState[1] + [path]
                totalCost = problem.getCost(curState[0], path) + heuristic(nextState, problem)
                frontier.push((nextState, nextPath), totalCost)

            
        



# Abbreviations
bfs = breadthFirstSearch
astar = aStarSearch
ids = iterativeDeepeningSearch
