#pragma once

namespace Magnum
{
    /* Implementation of Ken Shoemake's arcball camera with smooth navigation
        feature: https://www.talisman.org/~erlkonig/misc/shoemake92-arcball.pdf */
    class ArcBall
    {
    public:
        ArcBall(
            const Vector3& cameraPosition,
            const Vector3& viewCenter,
            const Vector3& upDir,
            Deg fov,
            Float nearPlane,
            Float farPlane,
            const Vector2i& windowSize);

        /* Set the camera view parameters: eye position, view center, up
            direction */
        void setViewParameters(const Vector3& eye, const Vector3& viewCenter, const Vector3& upDir);

        /* Reset the camera to its neighbors position, view center, and up dir */
        void reset();

        /* Update screen size after the window has been resized */
        void reshape(const Vector2i& windowSize) { _windowSize = windowSize; }

        /* Update any unfinished transformation due to lagging, return true if
            the camera matrices have changed */
        bool updateTransformation();

        /* Get/set the amount of lagging such that the camera will (slowly)
            smoothly navigate. Lagging must be in [0, 1) */
        Float lagging() const { return _lagging; }
        void setLagging(Float lagging);

        /* Initialize the first (screen) mouse position for camera
            transformation. This should be called in mouse pressed event. */
        void initTransformation(const Vector2& mousePos);

        /* Rotate the camera from the previous (screen) mouse position to the
            current (screen) position */
        void rotate(const Vector2& mousePos);

        /* Translate the camera from the previous (screen) mouse position to
            the current (screen) mouse position */
        void translate(const Vector2& mousePos);

        /* Translate the camera by the delta amount of (NDC) mouse position.
            Note that NDC position must be in [-1, -1] to [1, 1]. */
        void translateDelta(const Vector2& translationNDC);

        /* Zoom the camera (positive delta = zoom in, negative = zoom out) */
        void zoom(Float delta);

        /* Field of view */
        Deg fov() const { return _fov; }

        /* Get the camera's view transformation as a qual quaternion */
        const DualQuaternion& view() const { return _view; }

        /* Get the camera's view transformation as a matrix */
        Matrix4 viewMatrix() const { return _view.toMatrix(); }

        /* Get the camera's inverse view matrix (which also produces
            transformation of the camera) */
        Matrix4 inverseViewMatrix() const { return _inverseView.toMatrix(); }

        /* Get the camera's transformation as a dual quaternion */
        const DualQuaternion& transformation() const { return _inverseView; }

        /* Get the camera's transformation matrix */
        Matrix4 transformationMatrix() const { return _inverseView.toMatrix(); }

        /* Return the distance from the camera position to the center view */
        Float viewDistance() const { return Math::abs(_targetZooming); }

        auto fieldOfView() const { return _fov; }
        auto nearPlane() const { return _nearPlane; }
        auto farPlane() const { return _farPlane; }

    protected:
        /* Update the camera transformations */
        void updateInternalTransformations();

        /* Transform from screen coordinate to NDC - normalized device
            coordinate. The top-left of the screen corresponds to [-1, 1] NDC,
            and the bottom right is [1, -1] NDC. */
        Vector2 screenCoordToNDC(const Vector2& mousePos) const;

        Deg _fov;
        Vector2i _windowSize;

        Vector2 _prevMousePosNDC;
        Float _lagging{};
        Float _nearPlane, _farPlane;

        Vector3 _targetPosition, _currentPosition, _positionT0;
        Quaternion _targetQRotation, _currentQRotation, _qRotationT0;
        Float _targetZooming, _currentZooming, _zoomingT0;
        DualQuaternion _view, _inverseView;
    };
}

/* Arcball camera implementation integrated into the SceneGraph */
class viewer::camera::ArcBall : public Magnum::ArcBall
{
public:
    explicit ArcBall(
        const Vector3& cameraPosition,
        const Vector3& viewCenter,
        const Vector3& upDir,
        Magnum::Deg fov,
        Magnum::Float nearPlane,
        Magnum::Float farPlane,
        const Vector2i& windowSize,
        const Vector2i& viewportSize)
        : Magnum::ArcBall{ cameraPosition, viewCenter, upDir, fov, nearPlane, farPlane, windowSize }
    {
        /* Create a camera object of a concrete type */
        auto* cameraObject = new Object<MatrixTransformation3D>{ &scene_ };
        (*(camera_ = new Camera3D{ *cameraObject }))
            .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
            .setProjectionMatrix(Matrix4::perspectiveProjection(fov, 1.0f, nearPlane, farPlane))
            .setViewport(viewportSize);

        /* Save the abstract transformation interface and initialize the
            camera position through that */
        (*(cameraObject_ = cameraObject))
            .rotate(transformation().rotation())
            .translate(transformation().translation());
    }

    /* Update screen and viewport size after the window has been resized */
    void reshape(const Vector2i& windowSize, const Vector2i& viewportSize)
    {
        _windowSize = windowSize;
        camera_->setViewport(viewportSize);
    }

    /* Update the SceneGraph camera if arcball has been changed */
    bool update()
    {
        /* call the internal update */
        if (!updateTransformation())
            return false;

        (*cameraObject_)
            .resetTransformation()
            .rotate(transformation().rotation())
            .translate(transformation().translation());

        return true;
    }

    /* Draw objects using the internal scenegraph camera */
    void draw() { camera_->draw(drawables_); }

    /* Accessor to the raw camera object */
    Camera3D& camera() const { return *camera_; }

    DrawableGroup3D& drawables() { return drawables_; }
    Scene<MatrixTransformation3D>& scene() { return scene_; }

private:
    DrawableGroup3D drawables_{};
    Scene<MatrixTransformation3D> scene_;
    AbstractTranslationRotation3D* cameraObject_{};
    Camera3D* camera_{};
};
