from testpy import Exported

class ExportClass():
    def __init__(self):
        self.a = 0
        self.b = 2
        self.exported = Exported("hahaha,2131321")

    def set_a(self, v):
        self.a = v
    
    def set_b(self, v):
        self.b = v

    def print_msg(self):
        print(self.a, self.b, self.exported.getName())