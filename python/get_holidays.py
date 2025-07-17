from calendar import monthrange
from datetime import date, timedelta

def parse_date_flexible(d: str) -> date:
    try:
        parts = d.replace('-', '/').split('/')
        if len(parts) != 3:
            return None
        y, m, day = map(int, parts)
        return date(y, m, day)
    except Exception: return None

def get_holidays(year: int, months: int = None, include_weekends: bool = False, exclude_pastdays: bool = False) -> list:
    """
    Get a list of popular public holidays in Vietnam for a given year.
    Only holidays with a valid date and occurring today or later are included, and filtered by months ahead if months is not None.

    Args:
        year (int): The year for which to retrieve the list of holidays.
        months (int|None): Number of months ahead to include (None = full year).
        include_weekends (bool): Whether to include weekends (Saturday, Sunday) as holidays.
        exclude_pastdays (bool): Whether to exclude dates before today (default: True).

    Returns:
        list: A list of popular public holidays in Vietnam.
        Each containing 'name' (holiday name) and 'date' (string in yyyy/mm/dd format).
    """
    holidays = [
        {"name": "Tết Dương lịch", "date": "2025/01/01"},  # 🎉 Được nghỉ
        {"name": "Tết Nguyên Đán", "date": "2025/01/27"},  # 🎉 Được nghỉ (bắt đầu kỳ nghỉ Tết Âm lịch)
        {"name": "Giỗ tổ Hùng Vương", "date": "2025/04/07"},  # 🎉 Được nghỉ
        {"name": "Ngày Giải phóng miền Nam", "date": "2025/04/30"},  # 🎉 Được nghỉ
        {"name": "Ngày Quốc tế Lao động", "date": "2025/05/01"},  # 🎉 Được nghỉ
        {"name": "Ngày nghỉ hoán đổi (nghỉ)", "date": "2025/05/02"},  # 🎉 Được nghỉ (thay cho làm bù 26/04)
        {"name": "Ngày nghỉ hoán đổi (làm bù)", "date": "2025/04/26"},  # ❌ Đi làm bù
        {"name": "Ngày Quốc khánh", "date": "2025/09/02"},  # 🎉 Được nghỉ
        {"name": "Ngày Quốc khánh (nghỉ thêm)", "date": "2025/09/01"},  # 🎉 Được nghỉ
        # {"name": "Ngày Thầy thuốc Việt Nam", "date": "2025/02/27"},  # ❌ Không nghỉ
        # {"name": "Ngày Quốc tế Phụ nữ", "date": "2025/03/08"},  # ❌ Không nghỉ
        # {"name": "Ngày Quốc tế Thiếu nhi", "date": "2025/06/01"},  # ❌ Không nghỉ
        # {"name": "Ngày Nhà giáo Việt Nam", "date": "2025/11/20"},  # ❌ Không nghỉ
        # {"name": "Ngày thành lập Quân đội Nhân dân Việt Nam", "date": "2025/12/22"},  # ❌ Không nghỉ
    ]

    day = date.today() if exclude_pastdays else date(year=year, month=1, day=1)

    # Determine the end date for holidays (max: end of year).
    end_date = date(year, 12, 31)
    if isinstance(months, int) and months > 0:
        m = day.month + months
        y = day.year
        if m > 12:
            y += (m - 1) // 12
            m = (m - 1) % 12 + 1
        last_day = monthrange(y, m)[1]
        end_date = date(y, m, last_day)
        if end_date.year > year:
            end_date = date(year, 12, 31)

    # Add all Saturdays and Sundays from today to end_date as holidays if include weekends
    if include_weekends:
        weekends = []
        d = day
        while d <= end_date:
            if d.weekday() == 5:
                weekends.append({"name": "Cuối tuần (T7)", "date": d.strftime("%Y/%m/%d")})
            elif d.weekday() == 6:
                weekends.append({"name": "Cuối tuần (CN)", "date": d.strftime("%Y/%m/%d")})
            d += timedelta(days=1)
        holidays.extend(weekends)

    # Sort holidays by date
    holidays = [item for item in holidays if parse_date_flexible(item['date'])]
    holidays.sort(key=lambda h: parse_date_flexible(h['date']))

    # Filter days from today to the end day of year
    if exclude_pastdays:
        holidays = [item for item in holidays if day <= parse_date_flexible(item['date']) <= end_date]

    return holidays

from datetime import datetime

def is_dayoff_impl(dt: date | datetime) -> bool:
    """
    Check if a given date is a day off (holiday or weekend).

    Args:
        current_date (date|datetime): Date Time to check

    Returns:
        bool: True if the date is a holiday or weekend, False otherwise
    """
    holidays = get_holidays(year=dt.year, include_weekends=True, exclude_pastdays=False) or []

    holiday_date_strings = list({item['date'] for item in holidays})
    holiday_date_strings.sort()

    holiday_dates = [parse_date_flexible(item) for item in holiday_date_strings]

    day = dt if type(dt) is date else dt.date()

    return day in holiday_dates

def is_dayoff(dt_string: str) -> bool:
    """
    Check if a given date string is a day off (holiday or weekend).

    Args:
        dt_string (str): The date time string in format "%Y/%m/%d %H:%M:%S" or "%Y/%m/%d"

    Returns:
        bool: True if the date is a day off, False otherwise
    """
    try:
        if len(dt_string) > 10 and ':' in dt_string:
            dt = datetime.strptime(dt_string, "%Y/%m/%d %H:%M:%S")
        else:
            dt = datetime.strptime(dt_string, "%Y/%m/%d")
        return is_dayoff_impl(dt)
    except ValueError as e:
        print(f"Error: Invalid date format. Expected '%Y/%m/%d %H:%M:%S' or '%Y/%m/%d'")
    except Exception as e:
        print(f"Error: {str(e)}")
    return False

print("\n=== Test hàm check_dayoff ===")
test_dates = [
    "2025/07/07 12:00:00",  # Ngày thường (T72)
    "2025/01/01 09:00:00",  # Tết Dương lịch
    "2025/01/01",           # Tết Dương lịch (không có giờ)
    "2025/07/11 16:55:00",  # Ngày thường (T7)
    "2025/07/13",           # Chủ nhật
    "2025/04/30 14:30:00",  # Ngày Giải phóng miền Nam
]

for test_date in test_dates:
    try:
        result = is_dayoff(test_date)
        status = "ngày nghỉ" if result else "ngày làm việc"
        print(f"{test_date} là {status}")
    except Exception as e:
        print(f"Lỗi với {test_date}: {e}")
