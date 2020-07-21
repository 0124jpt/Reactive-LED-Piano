from mididings import *

config(
    backend='alsa',
    in_ports = [
        ('piano out', 'Digital Piano:0')
    ],
    out_ports = [
        ('LED in', 'Arduino Leonardo:0')
    ]
)

run(
    PortSplit({
        'piano out': [
            ChannelFilter(1) >> Output('LED in',1),
        ]
    })
)
