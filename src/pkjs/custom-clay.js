module.exports = function() {
  var clayConfig = this;
  var mediumLvl;
  var lowLvl;
  var debugText;

  function checkValueCorrect()
  { 
    if (parseInt(lowLvl.get()) >= parseInt(mediumLvl.get())) 
    {
      debugText.show();
      debugText.set("Low level can't be higher than medium level!");
    }
    else 
    {      
      debugText.hide();
    }     
  }
    
  
  //after page loaded
  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {    
    
    mediumLvl = clayConfig.getItemByMessageKey('battery_medium_lvl');
    lowLvl = clayConfig.getItemByMessageKey('battery_low_lvl');
    debugText = clayConfig.getItemByMessageKey('battery_level_warning');    
    
    debugText.hide();
    //toggleBackground.call(enableBackgroundToggle);
    mediumLvl.on('change', checkValueCorrect);
    lowLvl.on('change', checkValueCorrect);
    
   
  });
  

};