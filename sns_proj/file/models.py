from django.db import models

class FILE(models.Model):
    title = models.CharField(max_length=10)
    data = models.ImageField(upload_to='images/')

    def __str__(self):
        return self.title

