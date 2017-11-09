使用arduino使用按鍵控制音樂的播放動作,搭配TM1637 四位數顯示器顯元播放的音樂曲目,以及使用DFPlayer播放音樂<br>
TM1637 : http://atceiling.blogspot.tw/2017/03/arduino-rtc-tm1637.html <br>
DFPlayer : https://t93093.wordpress.com/2017/08/03/arduino-%E6%B8%AC%E8%A9%A6-dfplayer-mini-mp3-%E6%92%AD%E6%94%BE%E6%A8%A1%E7%B5%84/  <br>
<br><br><br><br>
腳位定義:<br>

TM1637 CLK PIN: 9<br>
TM1637 DIO PIN: 10<br>

DFPlayer BUSY PIN:13  ==> 判斷是否DPlayer在播放中<br> 
DFPlayer RX PIN:11<br>
DFPlayer TX PIN:12<br>

KeyPad:(2x5)<br>
Key rowPins = {2, 3}<br>
Key colPins = {4, 5, 6, 7, 8 }<br>

