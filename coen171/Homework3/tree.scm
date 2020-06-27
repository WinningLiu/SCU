; vi:ai:lisp:sm

; an empty node is represented as an empty list
; a nonempty node is represented as a list with three elements:
;    - the first element is the integer data
;    - the second element is the left child
;    - the third element is the right child


; Inserts values in a list into a tree

(define (insert tree node)
    (cond
        ((null? tree) 
            (list node '() '())
        )

        ((> (car tree) node) 
            (list
                (car tree) (insert (cadr tree) node)
                (caddr tree)
            )
        )

        (else 
            (list
                (car tree) (cadr tree)
                (insert (caddr tree) node)
            )
        )
    )
) 

(define (member? tree node)
    (cond 
        ((null? tree)
            #f    
        )
    
        ((< node (car tree))
            (member? (cadr tree) node)
        )

        ((> node (car tree))
            (member? (caddr tree) node)
        )

        (else
            #t
        )
    )
)

(define (inorder node)
    (cond
        ((null? node) '())

        (else 
            (list
                (inorder (cadr node)) 
                (car node) (inorder (caddr node))
            )
        )
    )
)

(define (helper x tree)
    (fold insert tree x))

(define (listToTree lst)
    (helper lst '())) 

(define (fold func val lst)
  (if (null? lst) val (fold func (func val (car lst)) (cdr lst))))

(define (build lst)
  (fold (lambda (t v) (insert t v)) '() lst))
