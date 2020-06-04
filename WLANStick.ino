/**
 *  PROJECT:    WLAN Stick
 *  AUTHOR:     FRANZ LORENZ
*/

/*************************************************
 * INCLUDE
 ************************************************/
#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPI.h>
#include <SD.h>

/*************************************************
 * CONST
 ************************************************/
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";
const char* host = "esp32sd";

/*************************************************
 * INCLUDE
 ************************************************/

WebServer server(80);
static bool hasSD = false;
File uploadFile;

/**
 * This function outputs the files and directories
 * of the root on the connected client.
 */
void printDirectory() 
{
  M5.Lcd.println( "- printDirectory()" ); 

  File dir = SD.open( "/" );  //slash is needed!
  server.sendContent( "<div style='margin:10px;border:2px solid gray;'>" );
  server.sendContent( "<table border='0' width='100%'>" );
  server.sendContent( "<tr style='background-color:lightgray'><td>Filename</td><td>Size</td><td>Action</td></tr>" );
  for( int cnt=0; true; ++cnt ) 
  {
    File entry = dir.openNextFile();
    String sName =  entry.name();
    sName = sName.substring( 1 );
    if( ! entry ) 
    {
      server.sendContent( "</table></div>" );
      break;
    }
    if( ! entry.isDirectory() )
    {
      String output;
      output = "<tr><td>";
      output += sName;
      output += "</td><td>";
      output += entry.size();
      output += "</td><td>";
      output += "<a href='?f=";
      output += sName;
      output += "'>Download</a>";
      output += "</td></tr>";
      server.sendContent( output );
    }
    entry.close();
  }  //for()
  dir.close();
}

/**
 * This function uploads the filename and
 * the content to the connected client of
 * the webserver.
 * @param  sFile   filename on sdcard
 */
void fileUpload( String sFile )
{
  unsigned long ulFileSize;
  int iPartialByteRead;
  M5.Lcd.print( "- fileUpload " ); 
  M5.Lcd.println( sFile ); 
  uploadFile = SD.open( "/"+sFile );
  server.sendHeader( "Content-Type", "text/text" );          
  server.sendHeader( "Content-Disposition", "attachment; filename="+sFile );
  server.sendHeader( "Connection", "close" );
  server.streamFile( uploadFile, "application/octet-stream" );
  uploadFile.close();
  server.client().stop();         //stop() must called, cause of "CONTENT_LENGTH_UNKNOWN"
}

/**
 * This function handles the request of the 
 * root website.
 */
void handleRoot() 
{
  if( server.args() == 1 )              //one parameter given?
  {
    if( server.argName( 0 ).equals( "f" ) )
    {
      fileUpload( server.arg( 0 ) );
    }
  }
  //
  server.setContentLength( CONTENT_LENGTH_UNKNOWN );
  server.send( 200, "text/html", "" );                        //send first
  server.sendContent( "<!DOCTYPE HTML><html lang='de'>" );
  server.sendContent( "<head>" );
  server.sendContent( "<meta charset='UTF-8'><meta name= viewport content='width=device-width, initial-scale=1.0,'>" );
  server.sendContent( "</head><body><div style='display:block;background-color:lightgreen'><h1>WLAN Stick</h1></div>" );
  printDirectory();
  server.sendContent( "</body>" );
  server.client().stop();         //stops, cause of "CONTENT_LENGTH_UNKNOWN"
}

/**
 * This function is called from arduino 
 * framework when the system started.
 */
void setup( void ) 
{
  M5.begin();
  M5.setWakeupButton(BUTTON_A_PIN);
  M5.Lcd.println( "SDWebServer is starting...");
  m5.Speaker.mute();
  //
  WiFi.softAP( ssid, password );
  M5.Lcd.print( "AccessPoint '" );
  M5.Lcd.print( ssid );
  M5.Lcd.print( "' '" );
  M5.Lcd.print( password );
  M5.Lcd.println( "' started..." ); 
  //
  IPAddress IP = WiFi.softAPIP();
  M5.Lcd.print( "AccessPoint IP address: " );
  M5.Lcd.println( IP );
  //
  server.on( "/", handleRoot );
  //
  server.begin();
  //
  if( SD.begin( SS ) ) 
  {
    M5.Lcd.println( "SD Card initialized..." );
    hasSD = true;
  }
  else
  {
    M5.Lcd.println( "SD Card PROBLEM!" );
  }
}

/**
 * This function is called from the arduino
 * framework after setup finished.
 */
void loop( void ) 
{
  if( M5.BtnA.wasPressed() ) 
  {
    M5.powerOFF();
  }
  server.handleClient();
  M5.update();
}
