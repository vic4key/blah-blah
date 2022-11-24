# coding: utf-8
from sqlalchemy import Column, Date, DateTime, Float, String, TIMESTAMP
from sqlalchemy.dialects.mysql import INTEGER, LONGTEXT, TINYTEXT
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()
metadata = Base.metadata


class Author(Base):
    __tablename__ = 'author'

    author_id = Column(INTEGER(11), primary_key=True, unique=True)
    name = Column(String(20), nullable=False)
    last_name = Column(String(50))
    initials = Column(String(4))
    orcid = Column(String(19))
    synonyms = Column(String(100))


class Clan(Base):
    __tablename__ = 'clan'

    clan_acc = Column(String(7), primary_key=True, unique=True)
    id = Column(String(40))
    previous_id = Column(TINYTEXT)
    description = Column(String(100))
    author = Column(TINYTEXT)
    comment = Column(LONGTEXT)
    created = Column(DateTime, nullable=False)
    updated = Column(TIMESTAMP, nullable=False)


class Version(Base):
    __tablename__ = 'version'

    rfam_release = Column(Float(4, True), primary_key=True)
    rfam_release_date = Column(Date, nullable=False)
    number_families = Column(INTEGER(10), nullable=False)
    embl_release = Column(TINYTEXT, nullable=False)
