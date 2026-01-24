Oefening 1:
a)
REATE TABLE Person (
    fullname VARCHAR(255) NOT NULL,
    PRIMARY KEY (fullname)
    -- Wands are linked to this table.
);

---

CREATE TABLE Magic_Wand (
    id INT NOT NULL,
    -- This column holds the name of the person currently using the wand.
    owner_fullname VARCHAR(255), 
    
    PRIMARY KEY (id),
    
    -- Default Foreign Key definition
    FOREIGN KEY (owner_fullname) REFERENCES Person(fullname)
);


