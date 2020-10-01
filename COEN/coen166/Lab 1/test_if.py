x = 1
if x:
    y = 2
    if y:
        print('block2')
    print('block1')
print('block0')

choice = 'ham'
if choice == 'spam':
    print(1.25)
elif choice == 'ham':
    print(1.99)
elif choice == 'eggs':
    print(0.99)
elif choice == 'bacon':
    print(1.10)
else:
    print('Bad choice')