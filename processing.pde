import processing.serial.*;

Serial myPort;
String data = "";
int angle = 0;
int distance = -1;

void setup() {
  size(900, 500);
  smooth();

  String[] ports = Serial.list();
  for (int i = 0; i < ports.length; i++) {
    println(i + " : " + ports[i]);
  }

  myPort = new Serial(this, ports[0], 9600);
  myPort.bufferUntil('\n');
}

void draw() {
  background(0);
  drawRadar();
  drawObject();
  drawText();
}

void serialEvent(Serial myPort) {
  data = myPort.readStringUntil('\n');
  if (data != null) {
    data = trim(data);
    String[] values = split(data, ',');
    if (values.length == 2) {
      angle = int(values[0]);
      distance = int(values[1]);
    }
  }
}

void drawRadar() {
  pushMatrix();
  translate(width/2, height);
  noFill();
  stroke(0, 255, 0);
  strokeWeight(2);

  arc(0, 0, 800, 800, PI, TWO_PI);
  arc(0, 0, 640, 640, PI, TWO_PI);
  arc(0, 0, 480, 480, PI, TWO_PI);
  arc(0, 0, 320, 320, PI, TWO_PI);
  arc(0, 0, 160, 160, PI, TWO_PI);

  for (int a = 0; a <= 180; a += 30) {
    float r = radians(a);
    line(0, 0, -400 * cos(r), -400 * sin(r));
  }
  popMatrix();
}

void drawObject() {
  if (distance >= 0 && distance <= 500) {
    pushMatrix();
    translate(width/2, height);
    stroke(255, 0, 0);
    strokeWeight(7);

    float r = map(distance, 0, 500, 0, 400);
    float rad = radians(angle);
    point(-r * cos(rad), -r * sin(rad));

    popMatrix();
  }
}

void drawText() {
  fill(0, 255, 0);
  textSize(20);
  text("Angle : " + angle + "°", 20, 30);

  if (distance == -1 || distance > 500) {
    text("Distance : Out of Range", 20, 60);
  } else if (distance == 0) {
    text("Distance : 000 cm", 20, 60);
  } else {
    text("Distance : " + distance + " cm", 20, 60);
  }
}
