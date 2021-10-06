INSERT into Customer VALUES('c1', 'John', 'Smith', 'SJ');
INSERT into Customer VALUES('c2', 'Mary', 'Jones', 'SFO');
INSERT into Customer VALUES('a1', 'Vincent', 'Chen', 'SJ');
INSERT into Customer VALUES('a12', 'Greg', 'King', 'SJ');
INSERT into Customer VALUES('c7', 'James', 'Bond', 'LA');
INSERT into Customer VALUES('x10', 'Susan', 'Blogg', 'SFO');

INSERT into DeliveryService VALUES('dsg1', 'grocery', 'SJ', 25.0);
INSERT into DeliveryService VALUES('dsb1', 'books', 'SJ',10.0);
INSERT into DeliveryService VALUES('dsm2', 'movies', 'LA', 10.0);
INSERT into DeliveryService VALUES('dby3', 'babygoods', 'SFO', 15.0);
INSERT into DeliveryService VALUES('dsg2', 'grocery', 'SFO', 20.0);
INSERT into DeliveryService VALUES('dg5', 'greengoods', 'SFO', 30.0);

INSERT into Schedule VALUES('dsg1', 'c1', 'm');
INSERT into Schedule VALUES('dsg1', 'a12', 'w');
INSERT into Schedule VALUES('dby3', 'x10', 'f');
INSERT into Schedule VALUES('dg5', 'c1', 'r');
INSERT into Schedule VALUES('dg5', 'c1', 't');
INSERT into Schedule VALUES('dg5', 'c32', 't');