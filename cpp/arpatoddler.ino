const long LOWEST_BPM = 30L;
const long HIGHEST_BPM = 220L;

const long MAX_ANALOG_READ = 1024L;


enum Notes {A=0, B, C, D, E, F, G, PAUSE};
enum NoteEventType {ON, OFF};

struct MusicEvent {
  Notes note;
  NoteEventType eventType;
  uint index;
};


class MusicEventListener {
public:
  ~MusicEventListener() = default;
  virtual void handleMusicEvent(MusicEvent event) = 0;
};

class BeatListener {
public:
  ~BeatListener() = default;
  virtual void handleBeat() = 0;
};

/* Receives beats and arpeggiates through a sequence of notes.
 * The sequence can be defined.
 * On each beat the arpeggiator can emmit note on / note off events
 */
class Arpeggiator : public BeatListener {
public:
  /* enables / disables arpeggiator */
  void toggle() { running = !running; }
  void cycle(uint index) {
    notes[index] = (notes[index] + 1) % PAUSE;
  }

  virtual void handleBeat() {
    uint next_note = (current_note + 1) % 8;
    for (auto listener : listeners) {
      listener.handleMusicEvent({notes[current_note], OFF, current_note});
      listener.handleMusicEvent({notes[next_note], ON, next_note});      
    }
    current_note = next_note;
  }

private:
  std::vector<MusicEventListener&> listeners;
  uint current_note;
  Note[8] notes;
  bool running;
};

/* This will be a wrapper for the sound chip. It will be receiving the Note on/ Note off events
it will also receive options for adjusting the sound texture pitch etc through apropriate interfaces */
class Synth : public MusicEventListener {};

/* Emits beats based on a controllable BPM */
class BeatHolder {
public:
  /* Expects a value between LOWEST_BPM and HIGHEST_BPM */
  void setTempo(uint bpm) {
    bpm_interval = 60L * 1000 / (2 * bpm);
  }
  /* Lets the beat holder to run its code and send beat events */
  void update() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis > bpm_interval) {
      previousMillis = currentMillis;
      beat_listener.handleBeat();
    }
  }
private:
  unsigned long previousMillis;
  unsigned long bpm_interval;
  BeatListener &beat_listener;
};

class Input {
public:
  virtual void check() = 0;
};

/* This is the main class that pulls together all the different components
 * of the arpatoddler 
 */
class Arpatoddler {
public:
  Arpatoddler() :

  /* Essentialy the arduino loop, only it has visibility to all
   * the arpatoddler's private members
   */
  void loop(){
    /* Check for beats */
    beat_holder.update();

    /* Check inputs */
    for (auto input : inputs) {
      input.check();
    }
  }

private:
  std::vector<Input> inputs;
  const LedArray &led_array;
  const Synth &synth;
  const Arpeggiator &arpeggiator;
  const BeatHolder &beat_holder; 
}

std::unique_ptr<Arpatoddler> arpatoddler;

void setup() {
  arpatoddler.reset(new Arpatoddler);
}

void loop()
{
  arpatoddler->loop();
}

