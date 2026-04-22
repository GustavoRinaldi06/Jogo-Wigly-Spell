#ifndef TIMER
#define TIMER

class Timer
{
    public:
        Timer();
        void Update(float dt);
        void Restart();
        float Get() const;
        void Set(float dt);

    private:
        float time;
};

#endif // Timer