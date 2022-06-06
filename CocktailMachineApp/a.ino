    String a = (String)per1 + "/" + (String)per2 + "/" + (String)per3 + "/" + (String)per4;
    a.toCharArray(buf, a.length() + 1);
    for (int i = a.length(); i < 20; i++)
      buf[i] = 'X';
    Serial.println(buf);
    Wire.write(buf);
