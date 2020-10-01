class FirstClass:
    def setdata(self, value1, value2): #Define class's methods
        self.data1 = value1 #self is the instance
        self.data2 = value2
    def display(self):
        print(self.data1, '\n', self.data2, '\n')

x = FirstClass() #make one instance

x.setdata("King Arthur", -5) #Call methods: self is x
x.display()

x.data1 = "QQ"
x.data2 = -3
x.display()

x.anothername = "spam"
x.display()
print(x.anothername)