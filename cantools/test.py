import cantools
from cantools.database import Signal, Message, Database
import os

vel_1 = Signal(
    name="joint1_vel",
    start=0,
    length=32,
    byte_order='little_endian'
)

vel_1.is_float = True


pos_1 = Signal(
    name="joint1_pos",
    start=32,
    length=32,
    byte_order='little_endian'
)

pos_1.is_float = True

msg = Message(
    frame_id=200,
    name="joint1",
    length=8,
    signals=[vel_1, pos_1]
)

db = Database()
db.messages.append(msg)
cantools.database.dump_file(db, 'arm.dbc')
print("Plik arm.dbc wygenerowany!")

os.system("python3 -m cantools generate_c_source arm.dbc")

print("pliki arm.c arm.h wygenerewanie")