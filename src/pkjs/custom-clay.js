module.exports = function() {
  var clayConfig = this;
  var mediumLvl;
  var lowLvl;
  var warning;

  function checkValueCorrect()
  { 
    if (parseInt(lowLvl.get()) >= parseInt(mediumLvl.get())) 
    {
      warning.show();
      warning.set("Low level can't be higher than medium level!");
    }
    else 
    {      
      warning.hide();
      warning.set("");
    }     
  }
    
  
  //after page loaded
  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {    
    
    mediumLvl = clayConfig.getItemByMessageKey('battery_medium_lvl');
    lowLvl = clayConfig.getItemByMessageKey('battery_low_lvl');
    warning = clayConfig.getItemByMessageKey('battery_level_warning');    
    
    warning.hide();
    //toggleBackground.call(enableBackgroundToggle);
    mediumLvl.on('change', checkValueCorrect);
    lowLvl.on('change', checkValueCorrect);
    
   
  });
  

};