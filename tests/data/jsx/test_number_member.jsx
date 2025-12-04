// Test cases for number member access bug

// Case 1: Direct number literal with toString
var test1 = (60).toString();

// Case 2: Binary expression result with toString
var test2 = (60 % 10).toString();

// Case 3: Modulo operation with toString (the reported bug)
var e = 3661;
var test3 = (Math.floor(e / 60) % 60).toString();

// Case 4: Multiple chained operations
var test4 = (10 + 20).toString();

// Case 5: Division result
var test5 = (100 / 5).toString();

// Case 6: The actual problematic pattern from user's code
var n = false;
var r = (n ? "-" : "") +
    (Math.floor(e / 3600) < 10 ? "0" + Math.floor(e / 3600) : Math.floor(e / 3600).toString()) +
    ":" +
    ((Math.floor(e / 60) % 60) < 10 ? "0" + (Math.floor(e / 60) % 60) : (Math.floor(e / 60) % 60).toString()) +
    ":" +
    (Math.floor(e % 60) < 10 ? "0" + Math.floor(e % 60) : Math.floor(e % 60).toString());
