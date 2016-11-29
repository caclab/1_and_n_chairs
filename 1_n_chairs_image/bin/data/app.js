var Scraper = require('images-scraper');
var bing = new Scraper.Bing();
var request = require('request');
var fs = require('fs');
var Jimp = require('jimp');
var exec = require('child_process').exec;

var q = process.argv[2];

function makeImgRequest(quer) {
    console.log("QUERY: " + quer);
    bing.list({
            keyword: quer,
            num: 10,
            detail: true
        })
        .then(function(res) {
            // console.log('first 10 results from bing', res);
            var index = Math.floor(Math.random() * 10);
            console.log("-----------------");
            console.log(res[index]);
            console.log("-----------------");
            console.log(res[index]['url']);
            var format = res[index]['format'];

            //Evaluate the image type.
            var checked = evaluateImageType(index, format, res).then(function(data) {
                if (data != 'jpg') {
                    Jimp.read(__dirname + "/image." + data).then(function(lenna) {
                        lenna.write(__dirname + "/image.jpg", function() {
                            exec("rm " + __dirname + "/image." + data, function(error, stdout, stderror) {
                                if (error) throw error;
                                console.log(stdout);
                                console.log(stderror);
                                process.exit();
                            });
                        });
                    }).catch(function(err) {
                        console.error(err);
                    });
                } else {
                    process.exit();
                }
            });
        }).catch(function(err) {
            //console.log('err', err);
            console.log(err + "\nNOTHING FOUND: retrying...");

            //cut out the last word of the query and retry...
            splitText(quer, function(d){
                makeImgRequest(d);
                q = d;
            })            
        })
}

//In case the query for bing image search throws no result, this function
//cuts out the last word and retries...
function splitText(t, callback){
    var arr = t.split('_');
    var removed = arr.splice(arr.length-1);
    var newString = arr.join('_');
    callback(newString);
}

var download = function(uri, filename, callback) {
    request.head(uri, function(err, res, body) {
        //if there is a request error or the image type is GIF
        //Also make sure the size of the image is les than 5MB
        if (!err && parseInt(res.headers['content-length']) < 500000){
            console.log("-----------------");
            console.log('content-type:', res.headers['content-type']);
            console.log('content-length:', res.headers['content-length']);
            console.log("=================");
            request(uri).pipe(fs.createWriteStream(filename)).on('close', callback);
        } else {
            console.log("ERROR: " + err + "\nretrying...");
            makeImgRequest(q);
        }
    });
};

//Evaluate the image type.
function evaluateImageType(index, f, res) {
    return {
        then: function(callback) {

            //Timeout event: if the request takes too long, retry 15 Secs later.
            var timeoutInterval = setTimeout(function() {
                console.log("TIMED-OUT: Retrying...");
                makeImgRequest(q);
            }, 15000);

            //The image is downloaded in its original filetype and in case it
            //is not a jpg or jpeg, then convert it and errase the original type.
            if(f == 'jpg' || f == 'jpeg' || f == 'png' || f == 'bmp'){
                download(res[index]['url'], __dirname + '/image.' + f, function() {
                    callback(f);
                    clearTimeout(timeoutInterval);
                });
            }else{
                //in case the image type not a jpg, jpeg, png or bmp
                makeImgRequest(q);
            }
        }
    };
}

makeImgRequest(q);
