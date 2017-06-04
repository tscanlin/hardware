var http = require("http");

var  on = false;

var options = {
  host: "192.168.1.20",
  path: "/CasaTunes/CasaService.svc/SetZonePower",
  method: "POST",
  data: {
    Power :on,
    ZoneID: 9
  }
};

setWatch(function(e) {
  console.log("Button pressed");
  on = !on;
  LED1.write(on);
  
  http.request(options, function(res) {
    res.on('data', function(data) {
      console.log(data);
    });
  });
  
}, BTN, { repeat: true, debounce:50, edge: 'rising' });
// you'll need to use  edge: 'falling' if you connected the button to 0v