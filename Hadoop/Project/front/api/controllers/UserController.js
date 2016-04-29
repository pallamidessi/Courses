/**
 * UserController
 *
 * @description :: Server-side logic for managing users
 * @help        :: See http://sailsjs.org/#!/documentation/concepts/Controllers
 */

var Twitter = require('twitter');

var client = new Twitter({
  consumer_key: sails.config.consumerKey,
  consumer_secret: sails.config.consumerSecret,
  access_token_key: sails.config.accessToken,
  access_token_secret: sails.config.accessTokenSecret
});

var fs = require('fs');
var fsExtra = require('fs-extra');
require('shelljs/global');


module.exports = {

  index: function(req, res){

    var q=req.param('q');
    var count=req.param('count');


    var fs = require('fs');
    var ret;


    if (q === 'undefined'){
      return res.view('500', {error: 'Query undefined'});
    }

    if (count === 'undefined'){
      return res.view('500', {error: 'Count undefined'});
    }

    client.get('search/tweets', {q: q, count: count}, function(error, tweets, response){
            if (count == 0) {
            return res.view('500', {error: 'No tweets'});
      }
      if (!error){

        var tweetsStr = '';

        for (var i in tweets.statuses)
        {
          tweetsStr += tweets.statuses[i].text + '\n';
        }

        fs.writeFileSync("input", tweetsStr);

        cp('-f', 'input', '../input');

        if (exec('../make.sh').code !== 0) {
          return res.view('500', {error: 'Error when calling hadoop script'});
        }

        var resFilename = '../output3/part-r-00000';

        cp('-f', '../output3/part-r-00000', 'part-r-00000');
        
        var sadResult = "0";
        var happyResult = "0";

        fs.readFile('part-r-00000', function (err, data) {
          if (err) {
            return res.view('500', {error: 'Error when reading file'});
          }
          console.log("file data" + data);
          data = data + " "
          var result = data.split("\n");
          console.log(result);
          sadResult = result[0].split("\t")[1];
          if (result.length > 1 && result[1] != ' ') {
            happyResult = result[1].split("\t")[1];
          }
          console.log(parseInt(sadResult));
          console.log(parseInt(happyResult));
        return res.view('tweets', { ret: tweetsStr, sad: sadResult, happy: happyResult });
        });


      }
    });
  }

};

