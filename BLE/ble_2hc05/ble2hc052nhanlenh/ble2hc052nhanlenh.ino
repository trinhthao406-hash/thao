

const int LED_VANG = 12;
const int LED_TRANG = 13;

void setup() {
  // Khai bao 2 chan LED la ngo ra
  pinMode(LED_VANG, OUTPUT);
  pinMode(LED_TRANG, OUTPUT);

  // Ban dau tat ca LED
  digitalWrite(LED_VANG, LOW);
  digitalWrite(LED_TRANG, HIGH);

  // Khoi dong UART ket noi voi HC2
  Serial.begin(9600);
}

void loop() {
  // Kiem tra co du lieu tu HC2 hay khong
  if (Serial.available() > 0) {
    char lenh = Serial.read();

    if (lenh == '\r' || lenh == '\n') {
      return;
    }
    if (lenh == 'A' || lenh == 'a') {
      digitalWrite(LED_VANG, HIGH);
      digitalWrite(LED_TRANG, LOW);

      Serial.println("A: CANH BAO");
    }

    else if (lenh == 'B' || lenh == 'b') {
      digitalWrite(LED_VANG, LOW);
      digitalWrite(LED_TRANG, HIGH);

      Serial.println("B: BINH THUONG");
    }

    
    
  }
}
