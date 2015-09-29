var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getWord() {
  // Construct URL
  var url = "http://learningwords.azurewebsites.net/api/words/random?from=EN&to=ES";

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      
      var fromWord = json.Word;
      console.log("fromWord is " + fromWord);
      var toWord = json.ToWord;
      console.log("toWord is " + toWord);
      var description = json.Description;
      console.log("description is " + description);
    
      
  
      
      
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_WORD": fromWord,
        "KEY_TOWORD": toWord,
       // "KEY_DESCRIPTION": description
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("LearningWords info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending words info to Pebble!");
        }
      );
    }      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWord();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWord();
  }                     
);
