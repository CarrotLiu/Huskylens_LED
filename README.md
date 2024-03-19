# Huskylens_LED
 Use huskylens face recognition for lighting FastLED.
 使用Huskylens进行人脸识别控制灯带示例


## 下载Arduino Hyskylens库
地址：[https://github.com/HuskyLens/HUSKYLENSArduino](https://github.com/HuskyLens/HUSKYLENSArduino)


## 识别到人脸后点亮灯带
```C
 else{
 Serial.println(F("###########"));
    if (huskylens.available())
    {
        HUSKYLENSResult result = huskylens.read();
        printResult(result);
        Serial.println(huskylens.available());
        //灯带点亮code .....
    }
 }
```

## 未识别到人脸熄灭灯带
```C
else if(!huskylens.available()){
    //灯带熄灭code .....
}
```
