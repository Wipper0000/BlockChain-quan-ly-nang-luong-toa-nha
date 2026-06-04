from pathlib import Path
p=Path(r'C:/Users/hungt/SmartCityEnergyESP8266/SmartCityEnergyESP8266.ino')
b=p.read_bytes()
print(b[:4].hex())
print('utf16' if len(b)>1 and b[1]==0 else 'utf8')
