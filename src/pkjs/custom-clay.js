module.exports = function() {
  var clayConfig = this;
  var mediumLvl;
  var lowLvl;
  var debugText;

  //we get auth key for user in clay settings  
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
    
    //background settings  - set to disable color pick when background image is enabled
    mediumLvl = clayConfig.getItemByMessageKey('battery_medium_lvl');
    lowLvl = clayConfig.getItemByMessageKey('battery_low_lvl');
    debugText = clayConfig.getItemByMessageKey('battery_level_warning');    
    
    debugText.hide();
    //toggleBackground.call(enableBackgroundToggle);
    mediumLvl.on('change', checkValueCorrect);
    lowLvl.on('change', checkValueCorrect);
    
    //disable date font radiogroup when date is disabled
    //var dateToggle = clayConfig.getItemByMessageKey('show_date');
    //toggleDateFont.call(dateToggle);
    //dateToggle.on('change', toggleDateFont);
   
  });
  

};