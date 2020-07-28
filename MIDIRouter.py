from mididings import *

config(
    backend='alsa',
    in_ports = [
        ('Controller Out', 'Digital Piano:0') # replace Digital Piano with whatever is the name of the MIDI controller you wish to use
    ],
    out_ports = [
        ('LED In', 'Arduino Leonardo:0') # replace Arduino Leonardo with the name of your MIDI Arduino
    ]
)

run(
    PortSplit({
        'piano out': [
            ChannelFilter(1) >> Output('LED in', 1), # channel filter is the MIDI channel of the MIDI controller
        ]                                           # replace '1' with the Arduino MIDI channel you want to route the data to
    })
)
