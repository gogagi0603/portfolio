def sugang_basic_answer():
    return "수강신청 페이지는 현재 미완성 입니다. 가까운 시일내로 찾아뵙겠습니다"

def descision_sugang(sent):
    if (("신청" in sent)):
        return sugang_basic_answer()
    else:
        return -1

def welfare_basic_answer():
    return "학생복지는 페이지는 현재 미완성입니다. 가까운 시일내로 찾아뵙겠습니다"

def descision_welfare(sent):
    if (("복지" in sent)):
        return welfare_basic_answer()
    else:
        return -1