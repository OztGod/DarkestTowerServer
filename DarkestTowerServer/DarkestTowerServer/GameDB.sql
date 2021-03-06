use Hmmo
go

SET NOCOUNT ON
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

IF EXISTS ( select * from sys.tables where name='AccountTable' )
	DROP TABLE [dbo].[AccountTable]
GO


CREATE TABLE [dbo].[AccountTable](
	[playerUID] [int] NOT NULL PRIMARY KEY IDENTITY(100, 1),
	[name] [nvarchar](32) NOT NULL DEFAULT (N'noname'),
	[password] [nvarchar] (32) NOT NULL,
	[win] [int] NOT NULL DEFAULT ((0)),
	[lose] [int] NOT NULL DEFAULT ((0)),
	[elo] [int] NOT NULL DEFAULT ((0)),
	[heroNum] [int] NOT NULL DEFAULT ((8)),
	[createTime] [datetime] NOT NULL,
	[isValid] [tinyint] NOT NULL,
	[comment] [nvarchar](256) NULL
)

GO

IF EXISTS ( select * from sys.tables where name='PlayerHeros' )
	DROP TABLE [dbo].[PlayerHeros]
GO

CREATE TABLE [dbo].[PlayerHeros](
	[heroId] [int] NOT NULL PRIMARY KEY IDENTITY(0, 1),
	[playerId] [int] NOT NULL,
	[heroType] [int] NOT NULL,
	[level] [int] NOT NULL,
	[exp] [int] NOT NULL,
	[maxHP] [int] NOT NULL,
	[hp] [int] NOT NULL,
	[hpGrow] [int] NOT NULL,
	[maxAP] [int] NOT NULL,
	[ap] [int] NOT NULL,
	[apGrow] [int] NOT NULL
)
GO

IF EXISTS ( select * from sys.tables where name='HeroSkills' )
	DROP TABLE [dbo].[HeroSkills]
GO

CREATE TABLE [dbo].[HeroSkills](
	[heroId] [int] NOT NULL,
	[skillType] [int] NOT NULL,
	[level] [int] NOT NULL
)

GO

-- info 관련 정보들

IF NOT EXISTS ( select * from sys.tables where name='HeroData' )
	CREATE TABLE [dbo].[HeroData](
		[heroType] [int] NOT NULL PRIMARY KEY,
		[hp] [int] NOT NULL,
		[hpGrow] [int] NOT NULL,
		[ap] [int] NOT NULL,
		[apGrow] [int] NOT NULL
	)
GO

IF NOT EXISTS ( select * from sys.tables where name='SkillData' )
	CREATE TABLE [dbo].[SkillData](
		[skillType] [int] NOT NULL PRIMARY KEY,
		[heroType] [int] NOT NULL,
		[value] [int] NOT NULL,
		[act] [int] NOT NULL,
		[cool] [int] NOT NULL
	)
GO



IF EXISTS ( select * from sys.procedures where name='spCreateAccount' )
	DROP PROCEDURE [dbo].[spCreateAccount]
GO

CREATE PROCEDURE [dbo].[spCreateAccount]
	@name	NVARCHAR(32),
	@password NVARCHAR(32)
AS
BEGIN
	SET NOCOUNT ON
	IF EXISTS ( SELECT * FROM AccountTable WHERE name = @name)
	BEGIN	
		SELECT 0
	END
	ELSE
	BEGIN	
		INSERT INTO AccountTable([name], [password], [createTime], [win], [lose], [isValid]) VALUES (@name, @password, GETDATE(), 0, 0, 1)
		SELECT @@IDENTITY
	END
END
GO

IF EXISTS ( select * from sys.procedures where name='spDeleteAccount' )
	DROP PROCEDURE [dbo].[spDeleteAccount]
GO

CREATE PROCEDURE [dbo].[spDeleteAccount]
	@playerUID	INT
AS
BEGIN
	SET NOCOUNT ON
	DELETE FROM AccountTable WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spPlayerWin' )
	DROP PROCEDURE [dbo].[spPlayerWin]
GO

CREATE PROCEDURE [dbo].[spPlayerWin]
	@playerUID	INT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE AccountTable SET win = win + 1 WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spPlayerLose' )
	DROP PROCEDURE [dbo].[spPlayerLose]
GO

CREATE PROCEDURE [dbo].[spPlayerLose]
	@playerUID	INT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE AccountTable SET lose = lose + 1 WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spUpdatePlayerValid' )
	DROP PROCEDURE [dbo].[spUpdatePlayerValid]
GO

CREATE PROCEDURE [dbo].[spUpdatePlayerValid]
	@playerUID	INT,
	@valid		TINYINT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE AccountTable SET isValid=@valid WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO


IF EXISTS ( select * from sys.procedures where name='spLoadPlayer' )
	DROP PROCEDURE [dbo].[spLoadPlayer]
GO

CREATE PROCEDURE [dbo].[spLoadPlayer]
	@name	NVARCHAR(255),
	@password NVARCHAR(255)
AS
BEGIN
	SET NOCOUNT ON
	SELECT [playerUID], [win], [lose], [elo], [heroNum], [isValid], [comment] FROM AccountTable WHERE [name] = @name AND [password]=@password AND isValid = 1
END		   
GO

IF EXISTS ( select * from sys.procedures where name='spLoadPlayerHeros')
	DROP PROCEDURE [dbo].[spLoadPlayerHeros]
GO

CREATE PROCEDURE [dbo].[spLoadPlayerHeros]
	@id INT
AS
BEGIN
	SET NOCOUNT ON
	SELECT [heroId], [heroType], [level], [exp], [maxHp], [maxAp], [hpGrow], [apGrow] FROM PlayerHeros WHERE [playerId] = @id
END
GO

IF EXISTS ( select * from sys.procedures where name = 'spUpdatePlayerHeros')
	DROP PROCEDURE [dbo].[spUpdatePlayerHeros]
GO

CREATE PROCEDURE [dbo].[spUpdatePlayerHeros]
	@heroId INT,
	@level INT,
	@exp INT,
	@maxHp INT,
	@maxAP INT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE PlayerHeros SET [level] = @level, [exp] = @exp, [maxHp] = @maxHp, [maxAp] = @maxAp WHERE [heroId] = @heroId 
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name = 'spInsertPlayerHeros')
	DROP PROCEDURE [dbo].[spInsertPlayerHeros]
GO

CREATE PROCEDURE [dbo].[spInsertPlayerHeros]
	@playerId INT,
	@heroType INT,
	@level INT,
	@maxHp INT,
	@maxAP INT,
	@hpGrow INT,
	@apGrow INT
AS
BEGIN
	SET NOCOUNT ON
	INSERT INTO PlayerHeros ([playerId], [heroType], [level],  [maxHp], [maxAp], [hp], [ap], [exp], [hpGrow], [apGrow]) VALUES (@playerId, @heroType, @level, @maxHp, @maxAP, @maxHp, @maxAp, 0, @hpGrow, @apGrow)
	SELECT @@IDENTITY
END
GO

IF EXISTS ( select * from sys.procedures where name = 'spLoadHeroSkills')
	DROP PROCEDURE [dbo].[spLoadHeroSkills]
GO

CREATE PROCEDURE [dbo].[spLoadHeroSkills]
	@heroId INT
AS
BEGIN
	SET NOCOUNT ON
	SELECT [skillType], [level] FROM HeroSkills where [heroId] = @heroId
END
GO

IF EXISTS ( select * from sys.procedures where name = 'spInsertHeroSkills')
	DROP PROCEDURE [dbo].[spInsertHeroSkills]
GO

CREATE PROCEDURE [dbo].[spInsertHeroSkills]
	@heroId INT,
	@skillType INT
AS
BEGIN
	SET NOCOUNT ON
	INSERT INTO HeroSkills([heroId], [skillType], [level]) VALUES (@heroId, @skillType, 1)
	SELECT @@IDENTITY
END
GO

IF EXISTS ( select * from sys.procedures where name='spLoadHeroData' )
	DROP PROCEDURE [dbo].[spLoadHeroData]
GO

CREATE PROCEDURE [dbo].[spLoadHeroData]
	@heroType	INT
AS
BEGIN
	SET NOCOUNT ON
	SELECT [hp], [hpGrow], [ap], [apGrow] FROM HeroData WHERE heroType=@heroType
END
GO

IF EXISTS ( select * from sys.procedures where name='spLoadSkillData' )
	DROP PROCEDURE [dbo].[spLoadSkillData]
GO

CREATE PROCEDURE [dbo].[spLoadSkillData]
	@skillType	INT
AS
BEGIN
	SET NOCOUNT ON
	SELECT [heroType], [value], [act], [cool] FROM SkillData WHERE skillType=@skillType
END	   
GO