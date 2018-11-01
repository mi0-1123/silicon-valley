#dataclass
class SushiParameter:
    def __init__(self, id_str, is_shake, is_stick, dist):
        self.id_str = id_str
        self.is_shake = is_shake == "1"
        self.is_stick = is_stick =="1"
        self.dist = float(dist)

