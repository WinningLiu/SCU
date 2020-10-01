import class3

rec3 = class3.Person('Jane', ['dev', 'mgr'], 30)

print(rec3.age)
print(rec3.info())

from class3 import Person

rec4 = Person('Mike', ['dev', 'mgr'], 35)
print(rec4.age)
print(rec4.info())