/*
* Decompiled with Jsxer
* Version: 1.7.5
* JSXBIN 2.0
*/

var e = 3661;
var n = false;
r = (n ? "-" : "") + (Math.floor(e / 3600) < 10 ? "0" + Math.floor(e / 3600) : Math.floor(e / 3600).toString()) + ":" + ((Math.floor(e / 60) % 60) < 10 ? "0" + (Math.floor(e / 60) % 60) : (Math.floor(e / 60) % 60).toString()) + ":" + (Math.floor(e % 60) < 10 ? "0" + Math.floor(e % 60) : Math.floor(e % 60).toString());
