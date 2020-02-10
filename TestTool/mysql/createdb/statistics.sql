CREATE TABLE statistics
(
   ts    TIMESTAMP NOT NULL,
   crate INT NOT NULL,
   run   INT NOT NULL,
   nmod  INT NOT NULL,
   rate  VARCHAR(64),
   stat  VARCHAR(32768)
);


