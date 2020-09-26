(define (filter f lst)
    (cond
        ((null? lst) '())
        ((f (car lst)) (cons (car lst) (filter f (cdr lst))))
        (else (filter f (cdr lst)))
    )
)

(define (quicksort f)
    (cond
        ((null? f) '())
        (else 
            (let*
                (
                    (h (car f))
                    (t (cdr f))
                    (below (lambda (x) (< x h)))
                    (above (lambda (x) (>= x h)))
                )
                (append (quicksort (filter below t)) (cons h (quicksort (filter above t))))
            )
        )
    ) 
)