//general stuff
String.prototype.replaceAll = function(search, replacement) {
    var target = this;
    return target.split(search).join(replacement);
};
Array.prototype.popFirst = function() {
    var first = this[0];
    this.splice(0,1);
    return first;
};
//end general stuff




var allData = {};
function getDataIntoMemory(data){
    var tmpAll = data.replaceAll("\n", " ").split(" ");
    allData['nrVideos'] = tmpAll.popFirst();
    allData['nrEndpoints'] = tmpAll.popFirst();
    allData['nrReqDesc'] = tmpAll.popFirst();
    allData['nrCaches'] = tmpAll.popFirst();
    allData['cacheCapacity'] = tmpAll.popFirst();
    allData['videosWeight'] = [];
    for(var i=0;i<parseInt(allData.nrVideos);i++){
        allData.videosWeight.push(tmpAll[i]);
    }
    tmpAll.splice(0, parseInt(allData.nrVideos));
    allData['endpoints'] = [];
    console.log(parseInt(allData.nrEndpoints));
    for(var i=0;i<parseInt(allData.nrEndpoints);i++){
        var tmpEndpoint = {};
        tmpEndpoint['cacheId'] = -1;//this is the datacenter
        tmpEndpoint['latency'] = parseInt(tmpAll.popFirst());
        allData.endpoints.push(tmpEndpoint);
        var endpointNrCaches = parseInt(tmpAll.popFirst());
        for(var j=0;j<endpointNrCaches;j++){
            tmpEndpoint['cacheId'] = tmpAll.popFirst();
            tmpEndpoint['latency'] = tmpAll.popFirst();
            allData.endpoints.push(tmpEndpoint);
        }
        if(i%100 === 0)
            console.log(i);
    }


    var fs = require('fs');
    var path = require('path');
    var filePath = path.join(__dirname, 'kittens.in');
    fs.writeFile(filePath, JSON.stringify(allData), function(err){
        console.log("Error while writing file: "+err);
    });
}

function doAllWork(data){
    getDataIntoMemory(data);
}
var fs = require('fs');
var path = require('path');
var filePath = path.join(__dirname, 'kittens.in');
fs.readFile(filePath, {encoding: 'utf-8'}, function(err,data){
    if (!err){
        doAllWork(data);
    }else{
        console.log(err);
    }

});