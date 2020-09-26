"Print the 10th Fibonacci number."

! Integer methodsFor: 'Fibonacci'!

fib
    self < 2
    ifTrue: [ ^1 ]
    ifFalse: [ ^(self - 1) fib + (self - 2) fib ]
!!

10 fib printNl!
