#include "buzzer.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

//

//const char* cuca = "pv:d=8,o=5,b=200:2p,p,c,c,c,4f.,4a,c,c,c,4f.,al,2a,p,4f,f,e,e,d,d,4c,4p.";
//const char* cuca_short = "pv:d=8,o=5,b=200:2p,p,c,c,c,4f.,4a,c,c,c,4f.,4a";
//const char* chiz = "chiz:d=8,o=5,b=200:e,c,e,c,f,e,d,p,g4,g4,g4,16a4,16b4,c,c,c,p";
//const char* nokia = "nokia:d=8,o=5,b=220:e6,d6,4f#,4g#,c#6,b,4d,4e,b,a,4c#,4e,1a";
//const char* barbie = "barbie:d=8,0=5,b=150:g#,e,g#,c#6,4a,4p,f#,d#,f#,b,4g#";
//const char* barbie2 = "barbie2:d=8,0=5,b=150:f#,d#,f#,b,2g#";
//const char* polonez1 = "polonez:d=8,o=5,b=120:4e.,16d#,16e,f,e,c,16cl,16b4,4a4";
//const char* polonez2 = "polonez:d=8,o=5,b=120:16d#,16e,f,e,c,16cl,16b4,4a4";
//const char* pv = "pv:d=8,o=5,b=120:c,eb,g,eb,4f,eb,d,4g,4f,2c,eb,g,bb,bb,4c6,bb,ab,2g,4a,4b,d6,c6,4g.,4d,c,g.,16f,2ab,bb,ab,4g,f,eb,4g,4f,2c";
//const char* pv_short = "pv_short:d=8,o=5,b=200:c,eb,g,eb,4f,eb,d,4g,4f,2c";
//const char* inter = "inter:d=8,o=5,b=200:4f,4bb.,a,c6,bb,f,d,2g,4eb";
//const char* mars_short = "mars:d=8,o=5,b=2000:16d,d.,16d,4g,4g,4a,4a,4d6.,b,g.";
//const char* mars = "mars:d=8,o=5,b=200:16d,d.,16d,4g,4g,4a,4a,4d6.,b,g.,16g,b.,16g,4e,2c6,a.,16f#,2g";
//const char* rt1 = "r1:d=8,o=6,b=400:c,e,g,e,c,g,e,g,c,g,c,e,c,g,e,g,e,c";
//const char* ii = "ii:d=16,o=5,b=320:d,p,d,p,d,p,g,p,g,p,g,p,d,p,d,p,d,p,a,p,a,p,a,2p";
//const char* rt_triple = "Triple:d=8,o=5,b=635:c,e,g,c,e,g,c,e,g,c6,e6,g6,c6,e6,g6,c6,e6,g6,c7,e7,g7,c7,e7,g7,c7,e7,g7";
//

//Note frequence table, an octave per line
static const int NoteFreqTable[][14] =
{
    // a   a#      b        c       c#   d    d#     e       f      f#    g   g#
    { 27,  29,    31,  16,  16,    17,  18,  19,    20, 22,  22,    23,  24, 26 },
    { 55,  58,    62,  32,  32,    34,  37,  39,    41, 43,  43,    46,  49, 52 },
    { 110, 116,   123, 65,  65,    69,  73,  78,    82, 87,  87,    92,  98, 103 },
    { 220, 233,   247, 130,  130,   138, 147, 155, 164, 174,  174,  185, 196, 207 },
    { 440, 466,   494, 261,  261,   277, 294, 311, 329, 349,  349,  370, 392, 415 },
    { 880, 932,   988, 523, 523,   554, 587, 622,  659, 698, 698,   740, 784, 830 },
    { 1760, 1865, 1976, 1047,1047, 1109, 1175,    1245, 1319, 1319, 1397, 1480, 1568, 1661 },
    { 3520, 3729, 3951, 2093, 2093, 2217, 2349,   2489, 2637, 2637, 2794, 2960, 3136, 3322 }
};


Buzzer::Buzzer()
{
    _volume = 100;
    _cyclic = false;
}


void Buzzer::PlayTone(uint16_t freq)
{
    uint32_t period = 1000000 / freq;
    //    _buzzerCh->UpdatePWM(period, _volume * 5);

 
}

void Buzzer::Stop()
{
    // _buzzerCh->UpdatePWM(0, 0);
}


void Buzzer::SetVolume(uint8_t vol) // 1 - 100
{
    _volume = vol;
    if (vol < 1)
    {
        _volume = 1;
    }
    else if (vol > 100)
    {
        _volume = 100;
    }

}


int Buzzer::find_char(const char* str, const char* c)
{
    int pos = 0;
    while (str[pos] != '\0')
    {
        if (str[pos] == c[0])
        {
            return pos;
        }
        else if (c[1] != 0 && str[pos] == c[1])
        {
            return pos;
        }
        ++pos;
    }

    return -1;
}

/* Parse header with default note duration, octave and tempo */
int16_t Buzzer::process_control(const char* tune)
{

    int16_t pos = 0;
    bool first_colon = true;
    while (true)
    {
        int pos0 = find_char(&tune[pos], "=:");

        if (pos0 == -1)
        {
            return -1;
        }

        pos += pos0;

        if (tune[pos] == ':')
        {
            if (!first_colon)
            {
                break;
            }
            else
            {
                first_colon = false;
            }
        }

        int num = atoi(&tune[pos + 1]);

        if (tune[pos - 1] == 'd')
        {
            _defaultDuration = num;
        }
        else if (tune[pos - 1] == 'o' && num > 0 && num < 8)
        {
            _defaultOctave = num;
        }
        else if (tune[pos - 1] == 'b')
        {
            _tempo = num;
        }
        ++pos;
    }

    return pos;

}

bool Buzzer::parse_note(const char* note_ptr, int* freq, int* duron_ms, int* duroff_ms)
{
    //1/2/4/8/16/32
    int noteType = atoi(note_ptr);

    if (noteType == 0)
    {
        noteType = _defaultDuration;
    }

    // Normalized duration based on the note type: 4 for 1/32th to 128 full note
    // It will next be converted to an absolute duration in ms based on the tempo
    int duration = 128 / noteType;

    while (isdigit(*note_ptr))
        note_ptr++;

    //Index into the frequency table based on the note name
    //It points to the column. The row is determined by the octave
    int note_freq_table_idx = -1;

    if (*note_ptr != 'p')
    {
        char c = tolower(*note_ptr++);

        //parse note name, valid letters are from A to G
        if (c < 'a' || c > 'g')
        {
            return false;
        }

        note_freq_table_idx = (c - 'a') * 2;
        if (*note_ptr == '#')
        {
            ++note_freq_table_idx;
            ++note_ptr;
        }
        else if (*note_ptr == 'b')
        {
            --note_freq_table_idx;
            ++note_ptr;
            if (note_freq_table_idx == -1)
            {
                note_freq_table_idx = 13;
            }
        }
    }
    else
    {
        //Rest, set frequency to zero
        ++note_ptr;
        *freq = 0;
    }

    if (note_freq_table_idx != -1)
    {
        //Parse octave
        int octave = atoi(note_ptr);
        if (octave == 0)
        {
            octave = _defaultOctave;
        }
        else
        {
            ++note_ptr;
        }

        *freq = NoteFreqTable[octave][note_freq_table_idx];
    }

    //Dotted noted, increase duration by half
    if (*note_ptr == '.')
    {
        duration += duration / 2;
        ++note_ptr;
    }

    // Calculate duration in milliseconds
    int dur_ms = 1000 * 60 / _tempo / 32 * duration;

    if (*freq != 0)
    {
        // legato, no gap, stay on for the entire dur_ms
        if (*note_ptr == 'l')
        {
            *duron_ms = dur_ms;
            *duroff_ms = 0;
        }
        // staccato, decrease the note on duration by half and assign the rest to silence
        else if (*note_ptr == 's')
        {
            *duroff_ms = dur_ms / 2;
            *duron_ms = dur_ms - *duroff_ms;
        }
        // normal case, insert a gap of 1/10 of the note duration  
        else {
            *duroff_ms = dur_ms / 10;
            *duron_ms = dur_ms - *duroff_ms;
        }
    }
    else //rest
    {
        *duroff_ms = dur_ms;
        *duron_ms = 0;
    }

    return true;
}

static void delay_us(int)
{

}

bool Buzzer::PlayTune(const char* tune)
{
    _defaultDuration = 8;
    _defaultOctave = 6;
    _tempo = 120;

    int pos = process_control(tune);
    if (pos == -1)
    {
        return false;
    }
    ++pos;

    int freq, duron_ms, duroff_ms;

    while (true)
    {
        //printf("pos: %d\n", pos);
        if (parse_note(&tune[pos], &freq, &duron_ms, &duroff_ms))
        {
            if (freq && duron_ms)
            {
                //printf("freq: %d\n", freq);
                //PlayTone(freq);
                Beep(freq, duron_ms);
            }

            //if (duron_ms)
            //{
            //    printf("duron: %d\n", duron_ms);
            //    delay_us(duron_ms * 1000);
            //}

            if (duroff_ms != 0)
            {
                //printf("duroff: %d\n", duroff_ms);
                //_buzzerCh->GetTimer()->EnableCounter(false);
                //_buzzerCh->SetupPWM1(0, 0);
                //_buzzerCh->UpdatePWM(0, 0);
                //delay_us(duroff_ms * 1000);
                Sleep(duroff_ms);
            }
        }
        else
        {
            printf("error parsing note\n");
        }

        int cpos = find_char(&tune[pos], ",");
        if (cpos == -1)
        {
            break;
        }
        pos += cpos + 1;
    }

    return true;
}






