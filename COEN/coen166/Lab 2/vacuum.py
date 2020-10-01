class roomba:
    def __init__(self, states, todo):
        self.states = states
        self.todo = todo
        self.score = 0
    def check(self):
        while self.states[0] == 1 or self.states[1] == 1:
            if self.states[0] == 1 and self.states[1] == 1 and self.states[2] == 0: #both dirty and roomba is left 110
                self.states[0] = 0
                self.todo.append("suck")
                self.states[2] = 1
                self.todo.append("move right")
                self.score += 2
            elif self.states[0] == 1 and self.states[1] == 1 and self.states[2] == 1: #both dirty and roomba is right 111
                self.states[1] = 0
                self.todo.append("suck")
                self.states[2] = 0
                self.todo.append("move left")
                self.score += 2
            elif self.states[0] == 0 and self.states[1] == 1 and self.states[2] == 1: #right dirty and roomba is right 011
                self.states[1] = 0
                self.todo.append("suck")
                self.score += 1
            elif self.states[0] == 1 and self.states[1] == 0 and self.states[2] == 0: #left dirty and roomba is left 100
                self.states[0] = 0
                self.todo.append("suck")
                self.score += 1
            elif self.states[0] ==  0 and self.states[1] == 1 and self.states[2] == 0: #right dirty, and roomba is left 010
                self.states[2] = 1
                self.todo.append("move right")
                self.states[1] = 0
                self.todo.append("suck")
                self.score += 2
            elif self.states[0] == 1 and self.states[1] == 0 and self.states[2] == 1: #left dirty, and roomba is right 101
                self.states[2] = 0
                self.todo.append("move left")
                self.states[0] = 0
                self.todo.append("suck")
                self.score += 2

#0 is clean, 1 is dirty
#For positions, 0 is left, 1 is right
#All possible states
#0, 0, 0 doesnt matter
#0, 0, 1 doesnt matter
#0, 1, 0 done
#1, 0, 0 done
#0, 1, 1 done
#1, 1, 0 done
#1, 0, 1 done
#1, 1, 1 done