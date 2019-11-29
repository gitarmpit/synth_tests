#include "buzzer.h"
#include <stdio.h>

int main(int argc, char**argv)
{

    Buzzer* b = Buzzer::GetInstance();
    if (!b->PlayTune(argv[1]))
    {
        printf("Error\n");
    }

    PlaySound(TEXT("recycle.wav"), NULL, SND_FILENAME);

    return 0;
}
