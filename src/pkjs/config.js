module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
    "type": "section",
    "capabilities": ["PLATFORM_BASALT"],
    "items": [
      {
      "type": "heading",
      "defaultValue": "Color"
      },
      {
      "type": "radiogroup",
      "messageKey": "screen_color",
      "defaultValue":"white",
      "options": [
        { 
          "label": "White", 
          "value": "white" 
        },
        { 
          "label": "Green", 
          "value": "green" 
        },
        { 
          "label": "Blue", 
          "value": "blue" 
        },
         { 
          "label": "Amber", 
          "value": "amber" 
        }
      ]
      }
    ]
  },
  {
    "type": "section",      
    "items": [
      {
      "type": "heading",
      "defaultValue": "Battery",
      "description": "Set levels on which vault boy will change."
      },
      {
        "type": "input",
        "messageKey": "battery_medium_lvl",
        "defaultValue": "50",
        "label": "Medium level",
        "attributes": {
          "type": "number",
          "max":"100",
          "min":"1"
        }
      },
      {
      "type": "input",
      "messageKey": "battery_low_lvl",
      "defaultValue": "20",
      "label": "Low level",
      "attributes": {
        "type": "number",
        "max":"100",
        "min":"1"
        }     
      },                 
      {
        "type": "text",
        "messageKey": "battery_level_warning",
        "defaultValue": ""
      }
    ]
  },
  {
    "type": "section",      
    "items": [
      {
      "type": "heading",
      "defaultValue": "Others"
      },
      {
        "type": "toggle",
        "messageKey": "vibe_disconnect",
        "label": "Vibrate on disconnect",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];