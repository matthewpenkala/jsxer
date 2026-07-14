var e = 3661; // example value for e (1 hour, 1 minute, 1 second)
var n = false; // example value for n
var r;

r = (n ? "-" : "") +
    (Math.floor(e / 3600) < 10 ? "0" + Math.floor(e / 3600) : Math.floor(e / 3600).toString()) +
    ":" +
    ((Math.floor(e / 60) % 60) < 10 ? "0" + (Math.floor(e / 60) % 60) : (Math.floor(e / 60) % 60).toString()) +
    ":" +
    (Math.floor(e % 60) < 10 ? "0" + Math.floor(e % 60) : Math.floor(e % 60).toString());
