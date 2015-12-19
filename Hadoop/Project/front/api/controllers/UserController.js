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


module.exports = {

  index: function(req, res){

    var q=req.param('q');
    var count=req.param('count');


    var fs = require('fs');
    var ret;


    if (q === 'undefined'){
      ret = 'Query undefined';
    }

    if (count === 'undefined'){
      ret = 'Count undefined';
    }

    client.get('search/tweets', {q: q, count: count}, function(error, tweets, response){

      if (!error){

        var tweetsStr = '';

        for (var i in tweets.statuses)
        {
          tweetsStr += tweets.statuses[i].text + '\n';
        }

        fs.writeFile("tweetToAnalyze.txt", tweetsStr, function(err) {
          if(err) {
            return console.log(err);
          }

          console.log("The file was saved!");
        });

        return res.view('tweets', { ret: tweetsStr })
      }
    });
  }

};

