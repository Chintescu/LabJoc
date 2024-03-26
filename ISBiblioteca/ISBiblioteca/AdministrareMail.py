import smtplib

def Trimitere_Email(email_expeditor, parola_expeditor, mesaj, email_destinatar):
    s = smtplib.SMTP('smtp.gmail.com', 587)
    s.starttls()
    s.login(email_expeditor, parola_expeditor)
    s.sendmail(email_expeditor, email_destinatar, mesaj)
    s.quit()

#parola trebuie sa fie generata cu apppassswords