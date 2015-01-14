var addon = require('bindings')('myaddon')

var interval = setInterval(function () {
	console.log('.')
}, 50)

addon.delayAsync(process.argv[2], function () {
  clearInterval(interval)
  console.log('Done!')
})

addon.call(function(msg){
	console.log(msg);
});

console.log('Waiting')