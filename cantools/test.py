import os
import cantools
from cantools.database import Database, Message, Signal


db = Database()

NUM_JOINTS = 6
BASE_FRAME_ID = 200

for i in range(NUM_JOINTS):
   
    vel_signal = Signal(
        name=f"joint{i}_vel",
        start=0,
        length=32,
        byte_order='little_endian',
        
    )
    vel_signal.is_float = True

    pos_signal = Signal(
        name=f"joint{i}_pos",
        start=32,
        length=32,
        byte_order='little_endian',
        
    )
    pos_signal.is_float = True    

    msg = Message(
        frame_id=BASE_FRAME_ID + i,
        name=f"joint{i}",
        length=8,
        signals=[vel_signal, pos_signal]
    )

    db.messages.append(msg)


cantools.database.dump_file(db, 'arm.dbc')
print("Plik arm.dbc wygenerowany!")

os.system("python3 -m cantools generate_c_source arm.dbc")
print("Pliki arm.c oraz arm.h zostały wygenerowane!")