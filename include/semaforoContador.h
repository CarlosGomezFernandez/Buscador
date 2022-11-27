/* Fragmento de código extraido del libro de la asignatura */

/******************************************************************************** 
* -Program: SSOOIIGLE Part-II (practica 3 de Sistemas Operativos II)
* -Author/s name: Jesus Baltasar Fernandez, Carlos Gomez Fernandez
* -Release/Creation date: 23/06/2021
* -Class name: semaforoContador.h
********************************************************************************/

class SemCounter{
  private:
    int value;
    std::mutex mutex_;
    std::mutex mutex_block;

    void block();
    void unblock();
    
  public:
    SemCounter(int value);  
    void wait();
    void signal();
    int getValue();
};

SemCounter::SemCounter(int v): value(v){};

void SemCounter::block(){mutex_block.lock();}
void SemCounter::unblock(){mutex_block.unlock();}

void SemCounter::wait(){
  mutex_.lock();
  if(--value <= 0){
    mutex_.unlock();
    block();
    mutex_.lock();
  }
  mutex_.unlock();
}

void SemCounter::signal(){
  mutex_.lock();
  if(++value <= 0){
    unblock();
    /*damos tiempo para que se realice el desbloqueo correctamente*/
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
  mutex_.unlock();
}

int SemCounter::getValue(){
  return value;
}