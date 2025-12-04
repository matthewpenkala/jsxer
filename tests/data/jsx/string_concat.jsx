var speedTime = 0.6;
var holdTime = 3.6;
var tempFadeTime = 0.2;

// Case 1: Simple addition in string concat
// Expected: "Test: 4.2"
// Buggy: "Test: 0.63.6"
var test1 = "Test: " + (speedTime + holdTime);

// Case 2: Chained addition
// Expected: "Test: 4.4"
// Buggy: "Test: 0.63.60.2"
var test2 = "Test: " + (speedTime + holdTime + tempFadeTime);

// Case 3: Mixed operations
// Expected: "Test: 4.0"
// Buggy: "Test: 0.63.60.2" (if precedence is totally broken)
var test3 = "Test: " + (speedTime + holdTime - tempFadeTime);

// Case 4: Complex expression from GoType
// Expected: "linear(_t_,4.4,..."
var test4 = "linear(_t_," + speedTime + holdTime + (speedTime - tempFadeTime) + "," + (speedTime * 2) + holdTime + ",-100,0)";

// Case 5: Nested in function call
alert("Result: " + (speedTime + holdTime));
