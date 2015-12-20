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

      if (!error){

        var tweetsStr = '';

        for (var i in tweets.statuses)
        {
          tweetsStr += tweets.statuses[i].text + '\n';
        }

        fs.writeFile("input", tweetsStr, function(err) {
          if(err) {
            console.log(err);
            return res.view('500', {error: 'Error when writing tweets file'});
          }

          console.log("The file was saved!");
        });


        cp('input', '../input');

        if (exec('../make.sh').code !== 0) {
          return res.view('500', {error: 'Error when calling hadoop script'});
        }

        var resFilename = '../output3/part-r-00000';

        cp('../output3/part-r-00000', 'part-r-00000');

        fs.readFile('part-r-00000', function (err, data) {
          if (err) {
            return res.view('500', {error: 'Error when reading file'});
          }
          console.log(data);
        });


        return res.view('tweets', { ret: tweetsStr, sad: 30, happy: 70 });
      }
    });
  }

};

